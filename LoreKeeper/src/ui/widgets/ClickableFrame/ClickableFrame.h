#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QPropertyAnimation>

namespace ui {

class ClickableFrame : public QFrame {
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    explicit ClickableFrame(QWidget *parent = nullptr);

    // 状态
    bool isSelected() const {
        return isSelected_;
    }
    bool isHighlighted() const {
        return isHighlighted_;
    }
    bool isEmpty() const {
        return isEmpty_;
    }

    // 设置状态
    void setSelected(const bool selected);
    void setHighlighted(const bool highlighted);
    void setEmpty(const bool empty);

    // 动画属性
    qreal opacity() const {
        return opacity_;
    }
    void setOpacity(qreal opacity);

    // 动画
    void animateClick();
    void animateHighlight();
    void animatePulse();

signals:
    void clicked();
    void doubleClicked();
    void mouseEntered();
    void mouseLeft();
    void rightClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void drawBackground(QPainter& painter);
    void drawBorder(QPainter& painter);
    void drawContent(QPainter& painter);

    bool isSelected_;
    bool isHighlighted_;
    bool isEmpty_;
    bool isHovered_;
    bool isPressed_;

    qreal opacity_;
    QPropertyAnimation* animation_;
};
} // namespace ui