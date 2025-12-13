//
// Created by bobobo on 2025/12/13.
//

#ifndef LOREKEEPER_PLAYER_H
#define LOREKEEPER_PLAYER_H

#include <QObject>
#include <QVector>
#include "../CardSystem/Card.h"

namespace core {

class Player : public QObject {
    Q_OBJECT

public:
    explicit Player(const QString& name, QObject* parent = nullptr);

    // 基本信息
    QString name() const;
    void setName(const QString& name);
    int health() const;
    int maxHealth() const;
    int mana() const;
    int maxMana() const;

    // 卡牌操作
    void drawCard();
    void addToHand(Card* card);
    void removeFromHand(int cardId);
    QVector<Card*> hand() const;

    // 战场操作
    bool playCardToField(int cardId, int slotIndex);
    Card* fieldCardAt(int slotIndex) const;

    // 状态修改
    void takeDamage(int amount);
    void heal(int amount);
    void addMana(int amount);
    void resetMana();

    // 牌库操作
    Deck* deck() const;
    Deck* graveyard() const;

signals:
    void healthChanged(int health);
    void manaChanged(int mana);
    void handChanged();
    void fieldChanged();

private:
    QString m_name;
    int32_t m_health;
    int32_t m_maxHealth;
    int32_t m_mana;
    int32_t m_maxMana;

    Deck* m_deck;
    Deck* m_graveyard;
    QVector<Card*> m_hand;
    QVector<Card*> m_field;
};
};

}

#endif //LOREKEEPER_PLAYER_H
