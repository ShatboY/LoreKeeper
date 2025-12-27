#pragma once

#include "BaseManager.h"
#include <QMap>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

namespace utils {

struct DeckData {
    QString name;
    QString description;
    QString playerClass;
    QMap<int32_t, int32_t> cards; // cardId -> count
    bool isDefault;
    QDateTime createdTime;
    QDateTime modifiedTime;
};
class DeckManager : public BaseManager {
    Q_OBJECT

public:
    explicit DeckManager(QObject *parent = nullptr);

    bool loadDeck(const QString &filePath = "");
    bool saveDeck(const QString &filePath = "");

    DeckData getDeck(const QString &deckName) const;
    bool saveDeck(const DeckData &deck);
    bool deleteDeck(const QString &deckName);
    bool duplicateDeck(const QString &sourceName, const QString &newName);
    bool renameDeck(const QString &oldName, const QString &newName);

    QStringList getAllDeckNames() const;
    QStringList getDecksByClass(const QString &playerClass) const;
    QStringList getDefaultDecks() const;
    QStringList getCustomDecks() const;

    DeckData createBasicDeck(const QString &playerClass);
    DeckData createRandomDeck(const QString &playerClass);

    bool validateDeck(const DeckData &deck, QString &errorMessage) const;

private:
    DeckData jsonToDeckData(const QJsonObject &json) const;
    QJsonObject deckDataToJson(const DeckData &deck) const;

    void generateDefaultDecks();

    QMap<QString, DeckData> deckDatabase_;

};

} // namespace utils
