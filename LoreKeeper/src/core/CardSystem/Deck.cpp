//
// Created by bobobo on 2025/12/21.
//

#include "Deck.h"

namespace core {
Deck::Deck(QObject *parent) : QObject(parent), name_("未命名卡组"), fatigueDamage_(1) {
}

Deck::Deck(const QString &name, QObject *parent) : QObject(parent), name_(name), fatigueDamage_(1) {
}

Deck::~Deck() {
    clear();
}

void Deck::clear() {
    cards_.clear();
    fatigueDamage_ = 1;
    emit deckChanged();
}

void Deck::addCard(const std::shared_ptr<Card> card) {
    if (!card) {
        return;
    }

    cards_.append(card);
    emit cardAdded(card);
    emit deckChanged();
}

void Deck::addCard(int32_t cardId, int32_t count) {

}

} // namespace core