//
// Created by bobobo on 2025/12/13.
//

#ifndef LOREKEEPER_PLAYER_H
#define LOREKEEPER_PLAYER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "../GameEngine/GameEngine.h"
#include "Deck.h"
#include "../CardSystem/Card.h"

namespace core {

class Player : public QObject {
    Q_OBJECT

public:
    explicit Player(const QString &name, QObject *parent = nullptr);
    ~Player() = default;

    // 基本信息
    QString name() const {
        return name_;
    }
    void setName(const QString &name) {
        name_ = name;
    }

    int32_t health() const {
        return health_value_;
    }
    int32_t maxHealth() const {
        return maxHealth_value_;
    }
    void setHealth(int32_t health);
    void setMaxHealth(int32_t maxHealth);

    int32_t mana() const {
        return mana_value_;
    }
    int32_t maxMana() const {
        return maxMana_value_;
    }
    void setMana(int32_t mana);
    void setMaxMana(int32_t maxMana);

    // 卡牌操作
    void drawCard(int32_t count = 1);
    void addToHand(std::shared_ptr<Card> card);
    bool removeFromHand(int32_t cardId);
    Card *handCard(int32_t cardId) const;
    QVector<std::shared_ptr<Card>> hand() const {
        return handcards_ptr_;
    }
    int32_t handSize() const {
        return handcards_ptr_.size();
    }

    // 战场操作
    bool playCardToField(const int32_t cardId, const int32_t slotIndex);
    bool playCardToField(const std::shared_ptr<Card> &card, const int32_t slotIndex);
    const std::shared_ptr<Card> &fieldCardAt(int32_t slotIndex) const;
    QVector<Card *> field() const;
    bool removeFromField(int32_t slotIndex);
    void resetFieldStatus();

    // 牌库操作
    std::unique_ptr<Deck> deck() const {
        return deck_ptr_;
    }
    std::unique_ptr<Deck> graveyard() const {
        return graveyard_ptr_;
    }
    void setDeck(Deck *deck);

    // 状态修改
    void takeDamage(int32_t amount);
    void heal(int32_t amount);
    void addMana(int32_t amount);

    // 效果相关
    void addStatus(const QString &status, int32_t duration);
    bool hasStatus(const QString &status) const;
    void removeStatus(const QString &status);

    // 查询接口
    int32_t deckSize() const;
    int32_t graveyardSize() const;
    bool isAlive() const {
        return health_value_ > 0;
    }

signals:
    void healthChanged(int32_t health, int32_t maxHealth);
    void manaChanged(int32_t mana, int32_t maxMana);
    void handChanged();
    void fieldChanged(int32_t slotIndex, Card *card);
    void deckChanged(int32_t size);
    void graveyardChanged(int32_t size);
    void playerDamaged(int32_t amount);
    void playerHealed(int32_t amount);
    void statusAdded(const QString &status);
    void statusRemoved(const QString &status);

private:
    QString name_;
    int32_t health_value_;
    int32_t maxHealth_value_;
    int32_t mana_value_;
    int32_t maxMana_value_;
    uint32_t field_size_;

    std::unique_ptr<Deck> deck_ptr_;
    std::unique_ptr<Deck> graveyard_ptr_;
    QVector<std::shared_ptr<Card>> handcards_ptr_;
    QMap<int32_t, std::shared_ptr<Card>> field_; // key: slot index, value: card

    QMap<QString, int32_t> statusEffects_; // status name -> duration
};
}; // namespace core

#endif // LOREKEEPER_PLAYER_H
