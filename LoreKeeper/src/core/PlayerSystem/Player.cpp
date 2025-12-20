//
// Created by bobobo on 2025/12/13.
//

#include "Player.h"
#include "Logger.h"
#include <algorithm>

namespace core {

Player::Player(const QString &name, QObject *parent) :
    QObject(parent),
    name_(name),
    health_value_(20),
    maxHealth_value_(20),
    mana_value_(0),
    maxMana_value_(0),
    field_size_(7),
    deck_ptr_(nullptr),
    graveyard_ptr_(nullptr) {
    deck_ptr_ = std::make_unique<Deck>(this);

    graveyard_ptr_ = std::make_unique<Deck>(this);

    // 初始化战场槽位
    for (uint32_t i = 0; i < field_size_; ++i) {
        field_.insert(i, nullptr);
    }
}

void Player::setHealth(int32_t health) {
    // 限制数值
    health = qBound(0, health, maxHealth_value_);
    if (health_value_ != health) {
        health_value_ = health;
        // 触发信号发射
        emit healthChanged(health_value_, maxHealth_value_);

        if (health <= 0) {
            //            Logger::info(QString("%1 has been defeated").arg(m_name));
        }
    }
}

void Player::setMana(int32_t mana) {
    mana = qBound(0, mana, maxMana_value_);
    if (mana_value_ != mana) {
        mana_value_ = mana;
        emit manaChanged(mana_value_, maxMana_value_);
    }
}

void Player::drawCard(int32_t count) {
    for (uint32_t i = 0; i < count; ++i) {
        if (deck_ptr_->isEmpty()) {
            // 疲劳机制
            takeDamage(deck_ptr_->failureDamage());
            emit gameLogAdded(QString("%1 疲劳受到 %2 点伤害").arg(name_).arg(deck_ptr_->failureDamage()));

            continue;
        }

        std::shared_ptr<Card> card{deck_ptr_->drawCard()};
        if (card) {
            addToHand(card);

            if (core::GameEngineGetInstance().currentPlayer() == this) {
                core::GameEngineGetInstance().addGameLog(QString("%1 抽了一张牌").arg(name_));
            }
        }
    }
}

void Player::addToHand(const std::shared_ptr<Card> card) {
    if (card) {
        return;
    }

    handcards_ptr_.append(card);
    emit handChanged();
}

bool Player::playCardToField(const int32_t cardId, const int32_t slotIndex) {
    // 从手帕找到卡牌
    auto it(std::find_if(handcards_ptr_.begin(), handcards_ptr_.end(),
                         [cardId](const std::shared_ptr<Card> &card) { return card->id() == cardId; }));
    if (it == handcards_ptr_.end()) {
        return false;
    }

    return playCardToField(*it, slotIndex);
}

bool Player::playCardToField(const std::shared_ptr<Card> &card, const int32_t slotIndex) {
    if ((!card) || (slotIndex < 0) || (slotIndex >= field_size_)) {
        return false;
    }

    // 检查槽位是否空闲
    if (field_[slotIndex] != nullptr) {
        return false;
    }

    // 从手牌移除
    handcards_ptr_.removeOne(card);

    // 放置到战场
    field_[slotIndex] = card;

    // 如果是本回合刚召唤的生物，标记为已行动
    if (card->isCreature()) {
        card->setExhausted(true);
    }

    emit handChanged();
    emit fieldChanged(slotIndex, card.get());

    return true;
}

const std::shared_ptr<Card> &Player::fieldCardAt(int32_t slotIndex) const {
    if ((slotIndex < 0) || (slotIndex >= field_size_)) {
        return nullptr;
    }

    auto it(field_.find(slotIndex));
    if (it != field_.end()) {
        return it.value();
    }

    return nullptr;
}

void Player::takeDamage(int32_t amount) {
    if (amount <= 0) {
        return;
    }

    setHealth(health_value_ - amount);
    emit playerDamaged(amount);

    // 触发护甲等效果
    // TODO: 实现相应效果
}

void Player::resetFieldStatus() {
    for (auto &pair : field_) {
        std::shared_ptr<Card> card = pair;
        if (card) {
            if (card->isCreature()) {
                // 重置攻击状态
                card->setExhausted(false);
            }
        }
    }
}
} // namespace core