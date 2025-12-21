#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QVector>
#include <QString>
#include <QPixmap>
#include <QMutex>
#include "../../core/CardSystem/Card.h"

namespace utils {

// 卡牌数据库结构
struct CardDatabaseEntry {
    int32_t id;
    QString name;
    QString description;
    Card::CardType type;
    Card::Rarity rarity;
    Card::CardClass cardClass;
    int32_t manaCost;
    int32_t attack;
    int32_t health;
    QVector<QString> keywords;
    QString effectScript;
    QString imagePath;
    QString flavorText;
};

// 牌组数据
struct DeckData {
    QString name;
    QString description;
    QString playerClass;
    QMap<int32_t, int32_t> cards; // cardId -> count
    bool isDefault;
    QDateTime createdTime;
    QDateTime modifiedTime;
};

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    static DatabaseManager *instance();
    ~DatabaseManager();

    // ============ 初始化与配置 ============
    bool initialize(const QString &dataPath = "");
    bool isInitialized() const {
        return m_initialized;
    }
    QString dataPath() const {
        return m_dataPath;
    }

    // ============ 卡牌数据库操作 ============
    bool loadCardDatabase(const QString &filePath = "");
    bool saveCardDatabase(const QString &filePath = "");
    bool reloadCardDatabase();

    // 卡牌创建与查询
    Card *createCardFromId(int32_t cardId, QObject *parent = nullptr) const;
    Card *createRandomCard(Card::CardClass cardClass = Card::CLASS_NEUTRAL,
                           Card::Rarity rarity = Card::RARITY_COMMON,
                           QObject *parent = nullptr) const;

    // 卡牌信息查询
    CardDatabaseEntry getCardEntry(int32_t cardId) const;
    bool cardExists(int32_t cardId) const;
    QVector<int32_t> getAllCardIds() const;
    QVector<int32_t> getCardsByClass(Card::CardClass cardClass) const;
    QVector<int32_t> getCardsByRarity(Card::Rarity rarity) const;
    QVector<int32_t> getCardsByType(Card::CardType type) const;
    QVector<int32_t> searchCards(const QString &keyword) const;

    // ============ 牌组管理 ============
    bool loadDeckDatabase();
    bool saveDeckDatabase();

    // 牌组操作
    DeckData getDeck(const QString &deckName) const;
    bool saveDeck(const DeckData &deck);
    bool deleteDeck(const QString &deckName);
    bool duplicateDeck(const QString &sourceName, const QString &newName);
    bool renameDeck(const QString &oldName, const QString &newName);

    // 牌组列表
    QStringList getAllDeckNames() const;
    QStringList getDecksByClass(const QString &playerClass) const;
    QStringList getDefaultDecks() const;
    QStringList getCustomDecks() const;

    // 牌组创建助手
    DeckData createBasicDeck(Card::CardClass playerClass);
    DeckData createRandomDeck(Card::CardClass playerClass);

    // ============ 游戏配置 ============
    bool loadGameConfig();
    bool saveGameConfig();

    QJsonObject getGameConfig() const;
    void setGameConfig(const QJsonObject &config);

    // 配置项快捷访问
    QString getStringConfig(const QString &key, const QString &defaultValue = "") const;
    int32_t getIntConfig(const QString &key, int32_t defaultValue = 0) const;
    bool getBoolConfig(const QString &key, bool defaultValue = false) const;
    QJsonArray getArrayConfig(const QString &key) const;
    QJsonObject getObjectConfig(const QString &key) const;

    void setStringConfig(const QString &key, const QString &value);
    void setIntConfig(const QString &key, int32_t value);
    void setBoolConfig(const QString &key, bool value);
    void setArrayConfig(const QString &key, const QJsonArray &value);
    void setObjectConfig(const QString &key, const QJsonObject &value);

    // ============ 玩家数据管理 ============
    bool loadPlayerData(const QString &playerName);
    bool savePlayerData(const QString &playerName);
    bool deletePlayerData(const QString &playerName);

    QJsonObject getPlayerData(const QString &playerName) const;
    void setPlayerData(const QString &playerName, const QJsonObject &data);

    QStringList getAllPlayers() const;

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

    // ============ 资源管理 ============
    QPixmap getCardImage(int32_t cardId, bool useCache = true);
    QPixmap getCardBackImage();
    QIcon getClassIcon(Card::CardClass cardClass);
    QIcon getRarityIcon(Card::Rarity rarity);

    bool preloadImages(const QVector<int32_t> &cardIds);
    void clearImageCache();

    // ============ 数据验证 ============
    bool validateDeck(const DeckData &deck, QString &errorMessage) const;
    bool validateCardEntry(const CardDatabaseEntry &entry, QString &errorMessage) const;

    // ============ 工具函数 ============
    static QString cardClassToString(Card::CardClass cardClass);
    static Card::CardClass stringToCardClass(const QString &str);
    static QString rarityToString(Card::Rarity rarity);
    static Card::Rarity stringToRarity(const QString &str);
    static QString cardTypeToString(Card::CardType type);
    static Card::CardType stringToCardType(const QString &str);

signals:
    void databaseLoaded();
    void databaseError(const QString &error);
    void deckAdded(const QString &deckName);
    void deckRemoved(const QString &deckName);
    void deckModified(const QString &deckName);
    void playerDataChanged(const QString &playerName);
    void configChanged();

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    // 初始化
    void setupDefaultPaths();
    bool createDirectories();
    bool loadDefaultCardDatabase();

    // 文件操作
    bool loadJsonFile(const QString &filePath, QJsonDocument &doc);
    bool saveJsonFile(const QString &filePath, const QJsonDocument &doc);

    // 数据处理
    CardDatabaseEntry jsonToCardEntry(const QJsonObject &json) const;
    QJsonObject cardEntryToJson(const CardDatabaseEntry &entry) const;
    DeckData jsonToDeckData(const QJsonObject &json) const;
    QJsonObject deckDataToJson(const DeckData &deck) const;

    // 默认数据生成
    void generateDefaultCards();
    void generateDefaultDecks();
    void generateDefaultConfig();

    static DatabaseManager *m_instance;

    bool m_initialized;
    QString m_dataPath;

    // 卡牌数据库
    QMap<int32_t, CardDatabaseEntry> m_cardDatabase;
    mutable QMutex m_cardDatabaseMutex;

    // 牌组数据库
    QMap<QString, DeckData> m_deckDatabase;
    mutable QMutex m_deckDatabaseMutex;

    // 游戏配置
    QJsonObject m_gameConfig;
    mutable QMutex m_configMutex;

    // 玩家数据
    QMap<QString, QJsonObject> m_playerData;
    mutable QMutex m_playerDataMutex;

    // 资源缓存
    QMap<int32_t, QPixmap> m_cardImageCache;
    QMap<Card::CardClass, QIcon> m_classIconCache;
    QMap<Card::Rarity, QIcon> m_rarityIconCache;
    mutable QMutex m_imageCacheMutex;

    // 默认图片
    QPixmap m_defaultCardImage;
    QPixmap m_cardBackImage;
};

} // namespace utils