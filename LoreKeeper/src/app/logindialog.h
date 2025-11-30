#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "../core/AuthManager/AuthManager.h"
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog() override;

private slots:
    void OnLoginClicked();
    void OnRegisterClicked();
    void OnForgotPasswordClicked();
    void UpdateLoginButtonState();

private:
    void SetupConnections();
    void LoadSavedCredentials();
    void SaveCredentials();
    bool ValidateInput();

private:
    std::unique_ptr<Ui::LoginDialog> login_w_ui_;

};
#endif // LOGINDIALOG_H
