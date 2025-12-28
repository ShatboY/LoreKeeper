#pragma once

#include "../BaseManager/BaseManager.h"
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <QJsonValue>

namespace utils {

class PlayerDataManager : public BaseManager {
    Q_OBJECT

public:
    explicit PlayerDataManager(QObject *parent = nullptr);

    bool loadPlayerData(const QString &filePath = "");
    bool savePlayerData(const QString &filePath = "");

    QJsonObject getPlayerData(const QString &playerName) const;
    void setPlayerData(const QString &playerName, const QJsonObject &data);

    QStringList getPlayerNames() const;

    // 玩家进度快捷访问
    int32_t getPlayerLevel(const QString &playerName) const;
    int32_t getPlayerExperience(const QString &playerName) const;
    int32_t getPlayerGold(const QString &playerName) const;
    QStringList getPlayerUnlockedCards(const QString &playerName) const;
    QStringList getPlayerUnlockedDecks(const QString &playerName) const;
    QJsonObject getPlayerStatistics(const QString &playerName) const;

    void updatePlayerStatistics(const QString &playerName,
                                const QString &statKey,
                                const QJsonValue &value);

private:
    QMap<QString, QJsonObject> playerData_;
};
} // namespace utils