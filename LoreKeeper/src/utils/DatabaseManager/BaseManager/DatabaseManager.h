#pragma once

#include "../CardDatabaseManager/CardDatabaseManager.h"
#include "../DeckManager/DeckManager.h"
#include "../GameConfigManager/GameConfigManager.h"
#include "../PlayerDataManager/PlayerDataManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "BaseManager.h"
#include <memory>
#include <QDir>
#include <QStandardPaths>

namespace utils {

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    static DatabaseManager &instance() noexcept;

    bool initialize(const QString &dataPath = "");
    bool isInitialized() const {
        return initialized_;
    }

    QString dataPath() const {
        return dataPath_;
    }

    // 获取各个管理器的引用
    CardDatabaseManager* cardManager() const {
        return cardManager_.get();
    }

    DeckManager* deckManager() const {
        return deckManager_.get();
    }

    GameConfigManager* gameConfigManager() const {
        return gameConfigManager_.get();
    }

    PlayerDataManager* playerDataManager() const {
        return playerDataManager_.get();
    }

    ResourceManager* resourceManager() const {
        return resourceManager_.get();
    }

signals:
    void databaseLoaded();
    void databaseError(const QString &error);

private:
    explicit DatabaseManager(QObject *parent = nullptr);

    DatabaseManager(const DatabaseManager &) = delete;

    DatabaseManager &operator=(const DatabaseManager &) = delete;

    DatabaseManager(DatabaseManager &&) = delete;

    DatabaseManager &operator=(DatabaseManager &&) = delete;

    ~DatabaseManager() = default;

    void setupDefaultPaths();
    bool createDirectories();

    bool initialized_{false};
    QString dataPath_;

    // 各个数据管理器实例
    std::unique_ptr<CardDatabaseManager> cardManager_;
    std::unique_ptr<DeckManager> deckManager_;
    std::unique_ptr<GameConfigManager> gameConfigManager_;
    std::unique_ptr<PlayerDataManager> playerDataManager_;
    std::unique_ptr<ResourceManager> resourceManager_;

};

static DatabaseManager &DatabaseManagerGetInstance() noexcept {
    return DatabaseManager::instance();
}
}