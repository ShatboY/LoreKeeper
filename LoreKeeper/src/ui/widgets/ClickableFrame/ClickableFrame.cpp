//
// Created by bobobo on 2025/12/21.
//

#include "ClickableFrame.h"
#include <QPainter>
#include <QLinearGradient>
#include <QGraphicsDropShadowEffect>

namespace ui {
ClickableFrame::ClickableFrame(QWidget *parent) :
    QFrame(parent),
    isSelected_(false),
    isHighlighted_(false),
    isEmpty_(true),
    isHovered_(false),
    isPressed_(false),
    opacity_(1.0),
    animation_(nullptr) {
    // 设置框架样式
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    // 允许鼠标跟踪
    setMouseTracking(true);

    // 设置默认样式
    setStyleSheet(
        "ClickableFrame {"
        "   background-color: rgba(100, 100, 100, 30);"
        "   border: 2px dashed #666;"
        "   border-radius: 5px;"
        "}"
        "ClickableFrame:hover {"
        "   border: 2px solid #4CAF50;"
        "   background-color: rgba(76, 175, 80, 30);"
        "}");

    // 创建动画
    animation_ = new QPropertyAnimation(this, "opacity");
    animation_->setDuration(300);
}

void ClickableFrame::setSelected(const bool selected) {
    if (isSelected_ != selected) {
        isSelected_ = selected;
        update();

        if (selected) {
            // 选中时添加阴影效果
            auto shadow{new QGraphicsDropShadowEffect(this)};
            shadow->setBlurRadius(20);
            shadow->setColor(QColor(255, 215, 0, 150)); // 金色阴影
            shadow->setOffset(0, 0);
            setGraphicsEffect(shadow);

            setStyleSheet(
                "ClickableFrame {"
                "   border: 3px solid #FFD700;"
                "   background-color: rgba(255, 215, 0, 50);"
                "   border-radius: 5px;"
                "}");
        } else {
            setGraphicsEffect(nullptr);
            setStyleSheet(
                "ClickableFrame {"
                "   background-color: rgba(100, 100, 100, 30);"
                "   border: 2px dashed #666;"
                "   border-radius: 5px;"
                "}"
                "ClickableFrame:hover {"
                "   border: 2px solid #4CAF50;"
                "   background-color: rgba(76, 175, 80, 30);"
                "}");
        }
    }
}

void ClickableFrame::setHighlighted(const bool highlighted)
{
    if (isHighlighted_ != highlighted) {
        isHighlighted_ = highlighted;
        update();

        if (highlighted) {
            setStyleSheet(
                "ClickableFrame {"
                "   border: 3px solid #FF5722;"
                "   background-color: rgba(255, 87, 34, 50);"
                "   border-radius: 5px;"
                "}"
            );

            // 开始脉冲动画
            animatePulse();
        } else {
            setStyleSheet(
                "ClickableFrame {"
                "   background-color: rgba(100, 100, 100, 30);"
                "   border: 2px dashed #666;"
                "   border-radius: 5px;"
                "}"
                "ClickableFrame:hover {"
                "   border: 2px solid #4CAF50;"
                "   background-color: rgba(76, 175, 80, 30);"
                "}"
            );

            // 停止动画
            if (animation_->state() == QAbstractAnimation::Running) {
                animation_->stop();
            }
            setOpacity(1.0);
        }
    }
}

void ClickableFrame::animatePulse()
{
    if (animation_->state() == QAbstractAnimation::Running) {
        animation_->stop();
    }

    animation_->setStartValue(0.5);
    animation_->setEndValue(1.0);
    animation_->setLoopCount(-1); // 无限循环
    animation_->setEasingCurve(QEasingCurve::InOutSine);
    animation_->start();
}

void ClickableFrame::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景
    drawBackground(painter);

    // 绘制边框
    drawBorder(painter);

    // 绘制内容（如果是空的，显示提示文字）
    drawContent(painter);
}

void ClickableFrame::drawBackground(QPainter& painter)
{
    QRect rect{this->rect().adjusted(1, 1, -1, -1)};

    // 创建渐变背景
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());

    if (isSelected_) {
        gradient.setColorAt(0, QColor(255, 215, 0, 30));
        gradient.setColorAt(1, QColor(255, 215, 0, 10));
    } else if (isHighlighted_) {
        gradient.setColorAt(0, QColor(255, 87, 34, 30));
        gradient.setColorAt(1, QColor(255, 87, 34, 10));
    } else if (isHovered_) {
        gradient.setColorAt(0, QColor(76, 175, 80, 30));
        gradient.setColorAt(1, QColor(76, 175, 80, 10));
    } else {
        gradient.setColorAt(0, QColor(100, 100, 100, 30));
        gradient.setColorAt(1, QColor(100, 100, 100, 10));
    }

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 5, 5);
}

void ClickableFrame::drawBorder(QPainter& painter)
{
    QRect rect{this->rect().adjusted(1, 1, -1, -1)};
    QPen pen;

    if (isSelected_) {
        pen = QPen(QColor(255, 215, 0), 3); // 金色
    } else if (isHighlighted_) {
        pen = QPen(QColor(255, 87, 34), 3); // 橙色
    } else if (isHovered_) {
        pen = QPen(QColor(76, 175, 80), 2); // 绿色
    } else {
        pen = QPen(QColor(102, 102, 102), 2, Qt::DashLine); // 灰色虚线
    }

    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect, 5, 5);
}

void ClickableFrame::drawContent(QPainter& painter)
{
    if (!isEmpty_) return;

    // 绘制提示文字
    painter.setPen(QColor(150, 150, 150, 150));
    QFont font("Microsoft YaHei", 10);
    painter.setFont(font);

    QString text{"空槽位"};
    QRect textRect{rect().adjusted(10, 10, -10, -10)};
    painter.drawText(textRect, Qt::AlignCenter, text);

    // 绘制加号图标
    int32_t centerX{width() / 2};
    int32_t centerY{height() / 2};
    int32_t crossSize{20};

    painter.setPen(QPen(QColor(150, 150, 150, 150), 2));

    // 水平线
    painter.drawLine(centerX - crossSize/2, centerY,
                     centerX + crossSize/2, centerY);
    // 垂直线
    painter.drawLine(centerX, centerY - crossSize/2,
                     centerX, centerY + crossSize/2);
}

void ClickableFrame::mousePressEvent(QMouseEvent* event)
{
    isPressed_ = true;
    update();

    // 点击动画
    animateClick();

    QFrame::mousePressEvent(event);
}

void ClickableFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if (isPressed_ && rect().contains(event->pos())) {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        } else if (event->button() == Qt::RightButton) {
            emit rightClicked();
        }
    }

    isPressed_ = false;
    update();

    QFrame::mouseReleaseEvent(event);
}

void ClickableFrame::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked();
    }

    QFrame::mouseDoubleClickEvent(event);
}

void ClickableFrame::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);

    isHovered_ = true;
    emit mouseEntered();
    update();
}

void ClickableFrame::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);

    isHovered_ = false;
    emit mouseLeft();
    update();
}

} // namespace ui