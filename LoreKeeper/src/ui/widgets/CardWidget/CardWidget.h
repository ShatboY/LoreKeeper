#pragma once

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include "../../../core/CardSystem/Card.h"

namespace ui {

class CardWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(float scale READ scale WRITE setScale)
    Q_PROPERTY(float opacity READ opacity WRITE setOpacity)

public:
    explicit CardWidget(core::Card *card, bool isInHand = true, QWidget *parent = nullptr);
    ~CardWidget();

    // 基本信息
    // 基本信息
    core::Card *card() const {
        return card_;
    }
    bool isInHand() const {
        return isInHand_;
    }

    // 状态
    bool isSelected() const {
        return isSelected_;
    }
    bool isPlayable() const {
        return isPlayable_;
    }
    bool isHighlighted() const {
        return isHighlighted_;
    }
    bool isTargetable() const {
        return isTargetable_;
    }

    // 设置状态
    void setSelected(const bool selected);
    void setPlayable(const bool playable);
    void setHighlighted(const bool highlighted);
    void setTargetable(const bool targetable);
    void setInHand(const bool inHand);

    // 动画属性
    float scale() const {
        return scale_;
    }
    void setScale(float scale);
    float opacity() const {
        return opacity_;
    }
    void setOpacity(float opacity);

    // 动画控制
    void animateDraw();
    void animatePlay();
    void animateAttack();
    void animateDamage(int32_t damage);
    void animateDestroy();
    void animateHoverEnter();
    void animateHoverLeave();

    // 尺寸
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void clicked(CardWidget *cardWidget);
    void doubleClicked(CardWidget *cardWidget);
    void mouseEntered(CardWidget *cardWidget);
    void mouseLeft(CardWidget *cardWidget);
    void cardPlayRequested(CardWidget *cardWidget);
    void cardAttackRequested(CardWidget *cardWidget);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void initializeUI();
    void updateAppearance();
    void createCardImage();
    void createGlowEffect();
    void createDamageEffect(int32_t damage);
    void updateToolTip();

    void drawCardFrame(QPainter &painter);
    void drawCardBackground(QPainter &painter);
    void drawCardName(QPainter &painter);
    void drawCardCost(QPainter &painter);
    void drawCardImage(QPainter &painter);
    void drawCardDescription(QPainter &painter);
    void drawCardStats(QPainter &painter);
    void drawCardRarity(QPainter &painter);
    void drawCardClass(QPainter &painter);
    void drawKeywords(QPainter &painter);

private:
    core::Card *card_;
    bool isInHand_;
    bool isSelected_;
    bool isPlayable_;
    bool isHighlighted_;
    bool isTargetable_;
    bool isHovered_;

    // 动画属性
    float scale_;
    float opacity_;

    // 卡牌图像
    QPixmap cardImage_;
    bool imageDirty_;

    // 效果
    QGraphicsDropShadowEffect *glowEffect_;
    QGraphicsOpacityEffect *opacityEffect_;
    QPropertyAnimation *scaleAnimation_;
    QPropertyAnimation *opacityAnimation_;

    // 颜色
    QColor frameColor_;
    QColor rarityColor_;
    QColor classColor_;

    // 尺寸常量
    static const int32_t CARD_WIDTH = 120;
    static const int32_t CARD_HEIGHT = 168;
    static const int32_t CARD_WIDTH_HOVER = 140;
    static const int32_t CARD_HEIGHT_HOVER = 196;
    static const int32_t CARD_WIDTH_FIELD = 100;
    static const int32_t CARD_HEIGHT_FIELD = 140;
};

} // namespace ui