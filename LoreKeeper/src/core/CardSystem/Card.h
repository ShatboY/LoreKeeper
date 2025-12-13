//
// Created by bobobo on 2025/12/13.
//

#ifndef LOREKEEPER_CARD_H
#define LOREKEEPER_CARD_H

#include <QObject>
#include <QString>
#include <QPixmap>

namespace core {

class Card : public QObject {
    Q_OBJECT

public:
    enum CardType {
        CREATURE,
        SPELL,
        ENCHANTMENT,
        ARTIFACT
    };

    enum Rarity {
        COMMON,
        UNCOMMON,
        RARE,
        EPIC,
        LEGENDARY
    };

    Card(int id, const QString& name, CardType type,
         const QString& description, Rarity rarity, int manaCost,
         QObject* parent = nullptr);

    // 基本信息
    int id() const;
    QString name() const;
    QString description() const;
    CardType type() const;
    Rarity rarity() const;
    int manaCost() const;

    // 生物卡特有属性
    bool isCreature() const;
    int attack() const;
    int defense() const;
    bool canAttack() const;
    bool hasTaunt() const;
    bool hasCharge() const;
    bool hasDivineShield() const;

    // 状态
    bool isExhausted() const;
    void setExhausted(bool exhausted);

    // 视觉效果
    QPixmap cardImage() const;
    QString cardFrameColor() const;

signals:
    void cardPlayed();
    void cardAttacked();
    void cardDestroyed();

protected:
    int m_id;
    QString m_name;
    QString m_description;
    CardType m_type;
    Rarity m_rarity;
    int m_manaCost;

    // 生物卡属性
    int m_attack;
    int m_defense;
    int m_currentDefense;

    // 状态标记
    bool m_isExhausted;
    bool m_hasTaunt;
    bool m_hasCharge;
    bool m_hasDivineShield;

    // 视觉效果
    QPixmap m_cardImage;
};

}



#endif //LOREKEEPER_CARD_H
