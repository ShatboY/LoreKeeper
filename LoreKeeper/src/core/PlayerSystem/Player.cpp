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
} // namespace core