#pragma once

#include <QObject>
#include <QMutex>
#include <QString>
#include <QJsonDocument>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace utils {

class BaseManager : public QObject {
    Q_OBJECT

protected:
    explicit BaseManager(QObject *parent = nullptr);

    // 共用的工具方法
    bool loadJsonFile(const QString &filepath, QJsonDocument &doc);
    bool saveJsonFile(const QString &filepath, const QJsonDocument &doc);

    QString getDataPath() const {
        return dataPath_;
    }

    void setDataPath(const QString &path) {
        dataPath_ = path;
    }

    bool isInitialized() const {
        return initialized_;
    }

    void setInitialized(bool initialized) {
        initialized_ = initialized;
    }

protected:
    QString dataPath_;
    bool initialized_ = false;
    mutable QMutex mutex_;
};

} // namespace utils