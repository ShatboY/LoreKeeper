#pragma once

#include "../BaseManager/BaseManager.h"
#include <QJsonObject>
#include <QJsonArray>

namespace utils {

class GameConfigManager : public BaseManager {
    Q_OBJECT

public:
    explicit GameConfigManager(QObject *parent = nullptr);

    bool loadConfig(const QString &filePath = "");
    bool saveConfig(const QString &filePath = "");

    QJsonObject getConfig() const;
    void setConfig(const QJsonObject &config);

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

private:
    void generateDefaultConfig();

    QJsonObject gameConfig_;
};

}