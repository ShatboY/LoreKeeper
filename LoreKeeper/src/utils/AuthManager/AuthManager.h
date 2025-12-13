//
// Created by bobobo on 2025/11/30.
//

#ifndef LOREKEEPER_AUTHMANAGER_H
#define LOREKEEPER_AUTHMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>

namespace  utils {

class AuthManager : public QObject {
    Q_OBJECT
public:
    static AuthManager& instance() noexcept;

    AuthManager();

    AuthManager(const AuthManager&) = delete;

    AuthManager& operator=(const AuthManager&) = delete;

    AuthManager(AuthManager&&) = delete;

    AuthManager& operator=(AuthManager&&) = delete;

    ~AuthManager() = default;

    bool ValidateUser(const QString& username, const QString& password);
    void SetCurrentUser(const QString& username) noexcept;
    QString GetCurrentUser() const;
    void ClearCurrentUser() noexcept;

    // 用户管理方法
    bool RegisterUser(const QString& username, const QString& password);
    bool ChangePassword(const QString& username, const QString& new_password);
    bool UserExists(const QString& username) const;

private:

    void LoadUsers();
    void SaveUsers();

private:
    QMap<QString, QString> m_users_;
    QString m_current_user_;

};

static AuthManager& AuthMangerGetInstance() noexcept {
    return AuthManager::instance();
}

}












#endif //LOREKEEPER_AUTHMANAGER_H
