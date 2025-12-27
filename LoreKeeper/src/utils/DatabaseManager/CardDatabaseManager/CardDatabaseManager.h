#pragma once

#include "../BaseManager/BaseManager.h"
#include "../../../core/CardSystem/Card.h"
#include <QMap>
#include <QVector>
#include <QString>

namespace utils {

struct CardDatabaseEntry {
    int32_t id;
    QString name;
    core::Card::CardType type;
    core::Card::Rarity rarity;
    core::Card::CardClass cardClass;
    int32_t manaCost;
    int32_t attack;
    int32_t health;
    QVector<QString> keywords;
    QString effectScript;
    QString imagePath;
    QString flavorText;
};

class CardDatabaseManager : public BaseManager {
    Q_OBJECT

public:
    explicit CardDatabaseManager(QObject *parent = nullptr);

    bool loadDatabase(const QString &filePath = "");
    bool saveDatabase(const QString &filePath = "");
    bool reloadDatabase();

    std::shared_ptr<core::Card> createCardFromId(int32_t cardId, QObject *parent = nullptr) const;
    std::shared_ptr<core::Card> createRandomCard(core::Card::CardClass cardClass = core::Card::CLASS_NEUTRAL,
                                                 core::Card::Rarity rarity = core::Card::RARITY_COMMON,
                                                 QObject *parent = nullptr) const;
    CardDatabaseEntry getCardEntry(int32_t cardId) const;
    bool cardExists(int32_t cardId) const;
    QVector<int32_t> getAllCardIds() const;
    QVector<int32_t> getCardsByClass(core::Card::CardClass cardClass) const;
    QVector<int32_t> getCardsByRarity(core::Card::Rarity rarity) const;
    QVector<int32_t> getCardsByType(core::Card::CardType type) const;
    QVector<int32_t> searchCards(const QString &keyword) const;

private:
    void generateDefaultCards();
    CardDatabaseEntry jsonToCardEntry(const QJsonObject &json) const;
    QJsonObject cardEntryToJson(const CardDatabaseEntry &entry) const;

    QMap<int32_t, CardDatabaseEntry> cardDatabase_;
};
} // namespace utils
