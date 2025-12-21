//
// Created by bobobo on 2025/12/21.
//

#include "BaseManager.h"

namespace utils {

BaseManager::BaseManager(QObject *parent) : QObject(parent) {
}

bool BaseManager::loadJsonFile(const QString &filepath, QJsonDocument &doc) {
    QFile file{filepath};
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonParseError error;
    doc = QJsonDocument::fromJson(data, &error);

    return error.error == QJsonParseError::NoError;
}

bool BaseManager::saveJsonFile(const QString &filepath, const QJsonDocument &doc) {
    QFile file{filepath};
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    return file.write(doc.toJson()) != -1;
}

} // namespace utils