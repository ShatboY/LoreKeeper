//
// Created by bobobo on 2025/12/21.
//

#include "DatabaseManager.h"

namespace utils {

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {
    // 初始化各个数据管理器
    cardManager_ = std::make_unique<CardDatabaseManager>(this);
    deckManager_ = std::make_unique<DeckManager>(this);
    gameConfigManager_ = std::make_unique<GameConfigManager>(this);
    playerDataManager_ = std::make_unique<PlayerDataManager>(this);
    resourceManager_ = std::make_unique<ResourceManager>(this);
}

bool DatabaseManager::initialize(const QString &dataPath) {
    setupDefaultPaths();
    if (!createDirectories()) {
        emit databaseError("无法创建数据目录");
        return false;
    }

    // 设置各个管理器的数据路径
    cardManager_->setDataPath(dataPath);
    deckManager_->setDataPath(dataPath);
    gameConfigManager_->setDataPath(dataPath);
    playerDataManager_->setDataPath(dataPath);
    resourceManager_->setDataPath(dataPath);

    initialized_ = true;
    emit databaseLoaded();
    return true;
}

void DatabaseManager::setupDefaultPaths() {
    if (dataPath_.isEmpty()) {
        dataPath_ = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/LoreKeeper";
    }
}

bool DatabaseManager::createDirectories() {
    QDir dir(dataPath_);
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}

DatabaseManager &DatabaseManager::instance() noexcept {
    static DatabaseManager instance;
    return instance;
}
} // namespace utils
