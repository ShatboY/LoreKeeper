#pragma once

#include <QObject>
#include <QVector>
#include <QRandomGenerator>
#include "Card.h"
#include "DatabaseManager.h"
#include "Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

namespace core {

class Deck : public QObject {
    Q_OBJECT

public:
    explicit Deck(QObject *parent = nullptr);
    explicit Deck(const QString &name, QObject *parent = nullptr);
    ~Deck();

    // 基本信息
    QString name() const {
        return name_;
    }
    void setName(const QString &name) {
        name_ = name;
    }

    // 卡牌操作
    void addCard(std::shared_ptr<Card> card);
    void addCard(int32_t cardId, int32_t count = 1);
    void removeCard(int32_t cardId, int32_t count = 1);
    std::shared_ptr<Card> drawTopCard();
    std::shared_ptr<Card> peekTopCard() const;
    void shuffle();
    void clear();

    // 状态查询
    bool isEmpty() const {
        return cards_.isEmpty();
    }
    int32_t size() const {
        return cards_.size();
    }
    int32_t cardCount(int32_t cardId) const;
    QVector<std::shared_ptr<Card>> cards() const {
        return cards_;
    }

    // 疲劳机制
    int32_t fatigueDamage() const {
        return fatigueDamage_;
    }
    void resetFatigue() noexcept {
        fatigueDamage_ = 1;
    }

    // 保存/加载
    bool saveToFile(const QString &filename) const;
    bool loadFromFile(const QString &filename);
    QJsonObject toJson() const;
    bool fromJson(const QJsonObject &json);

signals:
    void deckChanged();
    void cardAdded(std::shared_ptr<Card> card);
    void cardRemoved(std::shared_ptr<Card> card);
    void deckShuffled();

private:
    QString name_;
    QVector<std::shared_ptr<Card>> cards_;
    int32_t fatigueDamage_;

    void increaseFatigue() {
        fatigueDamage_++;
    }
};

} // namespace core