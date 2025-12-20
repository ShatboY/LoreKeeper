//
// Created by bobobo on 2025/12/13.
//

#include "Card.h"

namespace core {

Card::Card(const CardData &data, QObject *parent) :
    QObject(parent),
    cardData_(data),
    currentHealth_(data.health),
    currentAttack_(data.attack),
    isExhausted_(true), // 新召唤的生物不能立即攻击
    hasDivineShield_(data.keywords.contains("divine_shield")),
    isStealthed_(data.keywords.contains("stealth")),
    temporaryAttack_(0),
    temporaryHealth_(0) {
    // 初始化关键词集合
    for (const QString &keyword : data.keywords) {
        keywordSet_.insert(keyword);
    }

    // 如果有关键词charge，可以立即攻击
    if (hasCharge()) {
        isExhausted_ = false;
    }
}

bool Card::hasKeyword(const QString &keyword) const {
    return keywordSet_.contains(keyword);
}

bool Card::canAttack() const {
    if (!isCreature()) {
        return false;
    }

    return ((!isExhausted_) && (currentHealth_ > 0) && (!isStealthed_));
}

bool Card::canAttackImmediately() const {
    return (canAttack() && (!hasCharge()));
}

void Card::takeDamage(int32_t damage) {
    if (damage <= 0) {
        return;
    }

    // 神圣盾效果
    if (hasDivineShield_) {
        hasDivineShield_ = false;
        removeEffect("divine_shield");
        emit cardDamaged(0);
        return;
    }

    // 造成伤害
    currentHealth_ -= damage;
    emit cardDamaged(damage);

    // 检查死亡
    if (currentHealth_ <= 0) {
        emit cardDestroyed();
    }
}

void Card::heal(int32_t amount) {
    if (amount <= 0) {
        return;
    }

    int32_t oldHealth = currentHealth_;
    currentHealth_ = qMin(currentHealth_ + amount, cardData_.health);

    int32_t actualHeal = currentHealth_ - oldHealth;
    if (actualHeal > 0) {
        emit cardHealed(actualHeal);
    }
}

void Card::buff(int32_t attackBonus, int32_t healthBonus) {
    currentAttack_ += attackBonus;
    cardData_.attack += attackBonus;
    cardData_.health += healthBonus;
    currentHealth_ += healthBonus;

    emit cardBuffed(attackBonus, healthBonus);
}

QPixmap Card::cardImage() const {
    static QMap<CardClass, QString> classColors = {
        {CLASS_MAGE, "#0070dd"},
        {CLASS_WARRIOR, "#c41e3b"},
        {CLASS_PRIEST, "#ffffff"},
        {CLASS_ROGUE, "#fff569"},
        {CLASS_HUNTER, "#abd473"},
        {CLASS_WARLOCK, "#8788ee"},
        {CLASS_DRUID, "#ff7d0a"},
        {CLASS_SHAMAN, "#0070de"},
        {CLASS_PALADIN, "#f58cba"},
        {CLASS_NEUTRAL, "#8d8d8d"}};

    // 创建卡牌图像
    QPixmap pixmap(200, 280);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制卡牌背景
    QRect cardRect(0, 0, 200, 280);
    painter.setBrush(QColor(classColors.value(cardData_.cardClass, "#8d8d8d")));
    painter.setPen(Qt::black);
    painter.drawRoundedRect(cardRect, 10, 10);

    // 绘制卡牌名称区域
    QRect nameRect(10, 10, 180, 30);
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.drawRoundedRect(nameRect, 5, 5);

    // 绘制名称
    painter.setPen(Qt::white);
    QFont nameFont("Arial", 10, QFont::Bold);
    painter.setFont(nameFont);
    painter.drawText(nameRect, Qt::AlignCenter, cardData_.name);

    // 绘制法力值水晶
    QRect manaRect(10, 50, 30, 30);
    painter.setBrush(QColor(0, 100, 200));
    painter.drawEllipse(manaRect);

    painter.setPen(Qt::white);
    painter.drawText(manaRect, Qt::AlignCenter, QString::number(cardData_.manaCost));

    // 如果是生物， 绘制攻击/生命值
    if (isCreature()) {
        QRect attackRect(160, 240, 30, 30);
        painter.setBrush(QColor(200, 0, 0));
        painter.drawEllipse(attackRect);
        painter.drawText(attackRect, Qt::AlignCenter, QString::number(currentAttack_));

        QRect healthRect(10, 240, 30, 30);
        painter.setBrush(QColor(0, 200, 0));
        painter.drawEllipse(healthRect);
        painter.drawText(healthRect, Qt::AlignCenter, QString::number(currentHealth_));
    }

    // 绘制描述文本
    QRect descRect(20, 80, 160, 140);
    painter.setPen(Qt::black);
    QFont descFont("Arial", 8);
    painter.setFont(descFont);
    painter.drawText(descRect, Qt::TextWordWrap, cardData_.description);

    return pixmap;
}
} // namespace core