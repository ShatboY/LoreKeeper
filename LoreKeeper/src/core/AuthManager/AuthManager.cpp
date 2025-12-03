//
// Created by bobobo on 2025/11/30.
//

#include "AuthManager.h"
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>

namespace core {
AuthManager::AuthManager() {
    LoadUsers();

    // 如果没有用户，则创建默认管理员用户
    if (m_users_.empty()) {
        RegisterUser("admin", "123456");
    }
}

void AuthManager::LoadUsers() {
    QSettings settings;
    int32_t user_count = settings.beginReadArray("users");

    for (int32_t i = 0; i < user_count; ++i) {
        settings.setArrayIndex(i);
        QString username = settings.value("username").toString();
        QString password = settings.value("password").toString();
        m_users_[username] = password;
    }

    settings.endArray();
}

bool AuthManager::RegisterUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }

    if (m_users_.contains(username)) {
        // 用户已存在
        return false;
    }

    m_users_[username] = password;
    SaveUsers();
    return true;
}

void AuthManager::SaveUsers() {
    QSettings settings;
    settings.beginWriteArray("users");

    int32_t index = 0;
    for (auto it = m_users_.begin(); it != m_users_.end(); ++it) {
        settings.setArrayIndex(index++);
        settings.setValue("username", it.key());
        settings.setValue("password", it.value());
    }
    settings.endArray();
}

bool AuthManager::ValidateUser(const QString &username, const QString &password) {
    if (!m_users_.contains(username)) {
        return false;
    }

    // 简单密码验证
    QString store_password = m_users_[username];

    // 这里应该使用加密哈希
    return (store_password == password);

}

bool AuthManager::ChangePassword(const QString &username, const QString &new_password) {
    if (!m_users_.contains(username)) {
        return false;
    }

    m_users_[username] = new_password;
    SaveUsers();
    return true;
}

void AuthManager::SetCurrentUser(const QString &username) noexcept {
    m_current_user_ = username;
}

QString AuthManager::GetCurrentUser() const {
    return m_current_user_;
}

void AuthManager::ClearCurrentUser() noexcept {
    m_current_user_.clear();
}

bool AuthManager::UserExists(const QString &username) const {
    return m_users_.contains(username);
}

AuthManager& AuthManager::instance() noexcept {
    static AuthManager instance;
    return instance;
}

}