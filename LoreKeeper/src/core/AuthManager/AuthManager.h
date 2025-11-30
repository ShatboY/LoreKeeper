//
// Created by bobobo on 2025/11/30.
//

#ifndef LOREKEEPER_AUTHMANAGER_H
#define LOREKEEPER_AUTHMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>

namespace  core {

class AuthManager : public QObject {
    Q_OBJECT
public:
    static AuthManager& instance();

    bool ValidateUser(const QString& username, const QString& password);
    void SetCurrentUser(const QString& username);
    QString GetCurrentUser() const;
    void ClearCurrentUser();

    // 用户管理方法
    bool RegisterUser(const QString& username, const QString& password);
    bool ChangePassword(const QString& username, const QString& old_password, const QString& new_password);

private:
    AuthManager();
    void LoadUsers();
    void SaveUsers();

private:
    QMap<QString, QString> m_users_;
    QString m_current_user_;

};

static AuthManager& AuthMangerGetInstance() {
    return AuthManager::instance();
}

}












#endif //LOREKEEPER_AUTHMANAGER_H
