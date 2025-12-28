//
// Created by bobobo on 2025/12/21.
//

#include "CardWidget.h"
#include "../../../core/GameEngine/GameEngine.h"
#include "../../../utils/LoggerManager/Logger.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QLinearGradient>
#include <QRandomGenerator>

namespace ui {

CardWidget::CardWidget(core::Card *card, bool isInHand, QWidget *parent) :
    QWidget(parent),
    card_(card),
    isInHand_(isInHand),
    isSelected_(false),
    isPlayable_(false),
    isHighlighted_(false),
    isTargetable_(false),
    isHovered_(false),
    scale_(1.0f),
    opacity_(1.0f),
    imageDirty_(true),
    glowEffect_(nullptr),
    opacityEffect_(nullptr),
    scaleAnimation_(nullptr),
    opacityAnimation_(nullptr) {
    // 设置初始尺寸
    if (isInHand) {
        setFixedSize(CARD_WIDTH, CARD_HEIGHT);
    } else {
        setFixedSize(CARD_WIDTH_FIELD, CARD_HEIGHT_FIELD);
    }

    // 设置鼠标跟踪
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);

    // 初始化UI
    initializeUI();

    // 设置对象名用于样式表
    setObjectName("cardWidget");

    // 创建初始效果
    createGlowEffect();

    // TODO 添加日志
}

CardWidget::~CardWidget() {
    delete scaleAnimation_;
    delete opacityAnimation_;
}

void CardWidget::initializeUI() {
    if (!card_) {
        return;
    }

    // 根据卡牌类型设置颜色
    switch (card_->rarity()) {
    case core::Card::RARITY_COMMON:
        rarityColor_ = QColor("#808080"); // 灰色
        break;
    case core::Card::RARITY_UNCOMMON:
        rarityColor_ = QColor("#1eff00"); // 绿色
        break;
    case core::Card::RARITY_RARE:
        rarityColor_ = QColor("#0070dd"); // 蓝色
        break;
    case core::Card::RARITY_EPIC:
        rarityColor_ = QColor("#a335ee"); // 紫色
        break;
    case core::Card::RARITY_LEGENDARY:
        rarityColor_ = QColor("#ff8000"); // 橙色
        break;
    default:
        break;
    }

    // 根据职业设置颜色
    switch (card_->cardClass()) {
    case core::Card::CLASS_MAGE:
        classColor_ = QColor("#0070dd"); // 深蓝色
        frameColor_ = QColor("#69ccf0"); // 浅蓝色
        break;
    case core::Card::CLASS_WARRIOR:
        classColor_ = QColor("#c41e3b"); // 深红色
        frameColor_ = QColor("#c69b6d"); // 土黄色
        break;
    case core::Card::CLASS_PRIEST:
        classColor_ = QColor("#ffffff"); // 白色
        frameColor_ = QColor("#f0ebe0"); // 米白色
        break;
    case core::Card::CLASS_ROGUE:
        classColor_ = QColor("#fff569"); // 亮黄色
        frameColor_ = QColor("#fff569"); // 亮黄色
        break;
    case core::Card::CLASS_HUNTER:
        classColor_ = QColor("#abd473"); // 草绿色
        frameColor_ = QColor("#aad372"); // 相近的草绿色
        break;
    case core::Card::CLASS_WARLOCK:
        classColor_ = QColor("#8788ee"); // 紫色
        frameColor_ = QColor("#8788ee"); // 紫色
        break;
    case core::Card::CLASS_DRUID:
        classColor_ = QColor("#ff7d0a"); // 橙色
        frameColor_ = QColor("#ff7d0a"); // 橙色
        break;
    case core::Card::CLASS_SHAMAN:
        classColor_ = QColor("#0070de"); // 深蓝色
        frameColor_ = QColor("#0070de"); // 深蓝色
        break;
    case core::Card::CLASS_PALADIN:
        classColor_ = QColor("#f58cba"); // 粉色
        frameColor_ = QColor("#f48cba"); // 相近的粉色
        break;
    default:                             // NEUTRAL
        classColor_ = QColor("#8d8d8d"); // 灰色
        frameColor_ = QColor("#8d8d8d"); // 灰色
        break;
    }

    // 更新工具提示
    updateToolTip();
}

void CardWidget::paintEvent(const QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 绘制卡牌背景
    drawCardBackground(painter);

    // 绘制卡牌外观
    drawCardFrame(painter);

    // 绘制卡牌内容
    if (card_) {
        drawCardName(painter);
        drawCardCost(painter);

        if (isInHand_ || width() > CARD_WIDTH_FIELD) {
            drawCardImage(painter);
            drawCardDescription(painter);
        }

        if (card_->isCreature()) {
            drawCardStats(painter);
        }

        drawCardRarity(painter);
        drawCardClass(painter);

        if (card_->hasKeyword("taunt") || card_->hasKeyword("charge") || card_->hasKeyword("divine_shield")) {
            drawKeywords(painter);
        }
    }

    // 绘制选中状态
    if (isSelected_) {
        painter.setPen(QPen(QColor(255, 215, 0), 3)); // 金色边框
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
    }

    // 绘制可打出状态
    if (isPlayable_) {
        painter.setPen(QPen(QColor(76, 175, 80), 2)); // 绿色边框
        painter.setBrush(QColor(76, 175, 80, 30));    // 半透明绿色填充
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
    }

    // 绘制高亮状态
    if (isHighlighted_) {
        painter.setPen(QPen(QColor(255, 87, 34), 2)); // 橙色边框
        painter.setBrush(QColor(255, 87, 34, 30));    // 半透明橙色填充
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
    }

    // 绘制目标状态
    if (isTargetable_) {
        painter.setPen(QPen(QColor(255, 0, 0), 2)); // 红色边框
        painter.setBrush(QColor(255, 0, 0, 20));    // 半透明红色填充
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
    }
}

void CardWidget::drawCardBackground(QPainter &painter) {
    // 创建渐变背景
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(40, 40, 40));
    gradient.setColorAt(1, QColor(20, 20, 20));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 8, 8);
}

void CardWidget::drawCardFrame(QPainter &painter) {
    // 绘制卡牌外框
    QLinearGradient frameGradient(0, 0, width(), 0);
    frameGradient.setColorAt(0, frameColor_.lighter(120));
    frameGradient.setColorAt(0.5, frameColor_);
    frameGradient.setColorAt(1, frameColor_.darker(120));

    painter.setPen(QPen(frameGradient, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 8, 8);
}

void CardWidget::drawCardName(QPainter &painter) {
    if (!card_) {
        return;
    }

    // 绘制名称区域
    QRect nameRect(10, 10, width() - 20, 25);
    QLinearGradient nameGradient(nameRect.topLeft(), nameRect.bottomRight());
    nameGradient.setColorAt(0, QColor(0, 0, 0, 180));
    nameGradient.setColorAt(1, QColor(0, 0, 0, 120));

    painter.setBrush(nameGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(nameRect, 4, 4);

    // 绘制名称文本
    painter.setPen(Qt::white);
    QFont nameFont("Microsoft YaHei", isInHand_ ? 10 : 8, QFont::Bold);
    painter.setFont(nameFont);

    QString name{card_->name()};
    QFontMetrics metrics(nameFont);
    QString elidedName{metrics.elidedText(name, Qt::ElideRight, nameRect.width() - 10)};

    painter.drawText(nameRect, Qt::AlignCenter, elidedName);
}

void CardWidget::drawCardCost(QPainter &painter) {
    if (!card_) {
        return;
    }

    // 绘制法力值水晶
    int32_t crystalSize{isInHand_ ? 30 : 24};
    QRect crystalRect(width() - crystalSize - 10, 45, crystalSize, crystalSize);

    // 绘制水晶渐变
    QRadialGradient crystalGradient(crystalRect.center(), crystalSize / 2);
    crystalGradient.setColorAt(0, QColor(0, 150, 255, 200));
    crystalGradient.setColorAt(1, QColor(0, 50, 150, 200));

    painter.setBrush(crystalGradient);
    painter.setPen(QPen(QColor(255, 255, 255, 150), 1));
    painter.drawEllipse(crystalRect);

    // 绘制法力值文本
    painter.setPen(Qt::white);
    QFont costFont("Arial", isInHand_ ? 12 : 10, QFont::Bold);
    painter.setFont(costFont);
    painter.drawText(crystalRect, Qt::AlignCenter, QString::number(card_->manaCost()));
}

void CardWidget::drawCardStats(QPainter &painter) {
    if (!card_ || !card_->isCreature()) {
        return;
    }

    int32_t attack{card_->attack()};
    int32_t health{card_->health()};

    // 绘制攻击力（左下角）
    QRect attackRect(10, height() - 35, 30, 30);
    QLinearGradient attackGradient(attackRect.topLeft(), attackRect.bottomRight());
    attackGradient.setColorAt(0, QColor(200, 50, 50, 220));
    attackGradient.setColorAt(1, QColor(150, 30, 30, 220));

    painter.setBrush(attackGradient);
    painter.setPen(QPen(QColor(255, 200, 200), 1));
    painter.drawEllipse(attackRect);

    painter.setPen(Qt::white);
    QFont statFont("Arial", isInHand_ ? 12 : 10, QFont::Bold);
    painter.setFont(statFont);
    painter.drawText(attackRect, Qt::AlignCenter, QString::number(attack));

    // 绘制生命值（右下角）
    QRect healthRect(width() - 40, height() - 35, 30, 30);
    QLinearGradient healthGradient(healthRect.topLeft(), healthRect.bottomRight());
    healthGradient.setColorAt(0, QColor(50, 200, 50, 220));
    healthGradient.setColorAt(1, QColor(30, 150, 30, 220));

    painter.setBrush(healthGradient);
    painter.setPen(QPen(QColor(200, 255, 200), 1));
    painter.drawEllipse(healthRect);

    painter.setPen(Qt::white);
    painter.drawText(healthRect, Qt::AlignCenter, QString::number(health));

    // 如果生物受伤，显示伤害效果
    if (health < card_->health()) {
        painter.setPen(QPen(QColor(255, 0, 0, 150), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(healthRect.adjusted(-2, -2, 2, 2));
    }
}

void CardWidget::updateToolTip() {
    if (!card_) {
        setToolTip("");
        return;
    }

    QString toolTip{QString("<b>%1</b><br>").arg(card_->name())};
    toolTip += QString("法力值: %1<br>").arg(card_->manaCost());

    if (card_->isCreature()) {
        toolTip += QString("攻击力: %1<br>").arg(card_->attack());
        toolTip += QString("生命值: %1/%2<br>").arg(card_->currentHealth()).arg(card_->health());
    }

    // 添加关键词
    QVector<QString> keywords{};
    if (card_->hasTaunt()) {
        keywords << "嘲讽";
    }
    if (card_->hasCharge()) {
        keywords << "冲锋";
    }
    if (card_->hasDivineShield()) {
        keywords << "圣盾";
    }
    if (card_->hasWindfury()) {
        keywords << "风怒";
    }
    if (card_->hasStealth()) {
        keywords << "潜行";
    }
    if (card_->hasPoisonous()) {
        keywords << "剧毒";
    }
    if (card_->hasLifesteal()) {
        keywords << "吸血";
    }
    if (card_->hasRush()) {
        keywords << "突袭";
    }

    if (!keywords.isEmpty()) {
        toolTip += "<br><b>关键词:</b><br>";
        for (const QString &keyword : keywords) {
            toolTip += QString("• %1<br>").arg(keyword);
        }
    }

    // 添加描述
    if (!card_->description().isEmpty()) {
        toolTip += QString("<br><i>%1</i>").arg(card_->description());
    }

    setToolTip(toolTip);
}

void CardWidget::setSelected(const bool selected) {
    if (isSelected_ != selected) {
        isSelected_ = selected;
        update();

        if (selected) {
            // 放大选中卡牌
            if (isInHand_) {
                setFixedSize(CARD_WIDTH_HOVER, CARD_HEIGHT_HOVER);
                raise(); // 提升到最前面
            }
        } else {
            // 恢复原尺寸
            if (isInHand_) {
                setFixedSize(CARD_WIDTH, CARD_HEIGHT);
            }
        }
    }
}

void CardWidget::setPlayable(const bool playable) {
    if (isPlayable_ != playable) {
        isPlayable_ = playable;
        update();

        // 创建发光效果
        if (playable && isPlayable_) {
            glowEffect_->setColor(QColor(76, 175, 80)); // 绿色发光
            glowEffect_->setBlurRadius(15);
            glowEffect_->setEnabled(true);
        } else if (glowEffect_) {
            glowEffect_->setEnabled(false);
        }
    }
}

void CardWidget::animateDraw() {
    if (!scaleAnimation_) {
        scaleAnimation_ = new QPropertyAnimation(this, "scale");
    }

    // 从0缩放到1
    scaleAnimation_->setDuration(500);
    scaleAnimation_->setStartValue(0.0f);
    scaleAnimation_->setEndValue(1.0f);
    scaleAnimation_->setEasingCurve(QEasingCurve::OutBack);
    scaleAnimation_->start();
}

void CardWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }

    QWidget::mousePressEvent(event);
}

void CardWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(this);

        // 双击打出卡牌
        if (isPlayable_ && isInHand_) {
            emit cardPlayRequested(this);
        }
    }

    QWidget::mouseDoubleClickEvent(event);
}

void CardWidget::enterEvent(QEnterEvent *event) {
    Q_UNUSED(event);

    isHovered_ = true;
    emit mouseEntered(this);

    // 手牌中的卡牌悬停放大
    if (isInHand_ && !isSelected_) {
        setFixedSize(CARD_WIDTH_HOVER, CARD_HEIGHT_HOVER);
        raise();
    }

    update();
}

void CardWidget::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);

    isHovered_ = false;
    emit mouseLeft(this);

    // 恢复原尺寸
    if (isInHand() && !isSelected_) {
        setFixedSize(CARD_WIDTH, CARD_HEIGHT);
    }

    update();
}

} // namespace ui
