#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , login_w_ui_(std::make_unique<Ui::LoginDialog>())
{
    login_w_ui_->setupUi(this);

    setWindowTitle("LoreKeeper - Login");

    // 设置密码框的回显模式
    login_w_ui_->Password_lineEdit->setEchoMode(QLineEdit::Password);

    // 设置连接
    utils::ConnectManager::SetupConnections(this, login_w_ui_.get());

    LoadSavedCredentials();

}

LoginDialog::~LoginDialog() = default;

void LoginDialog::OnLoginClicked() {
    if (!ValidateInput()) {
        return;
    }

    QString username = login_w_ui_->UserName_lineEdit->text().trimmed();
    QString password = login_w_ui_->Password_lineEdit->text();

    utils::AuthManager& auth_manager = utils::AuthMangerGetInstance();

    // 使用AuthManager验证凭据
    if (auth_manager.ValidateUser(username, password)) {
        // 登录成功
        // 保存凭据（如果勾选了”记住我“）
        if (login_w_ui_->Remember_checkBox->isChecked()) {
            SaveCredentials();
        } else {
            // 清除保存的凭据
            QSettings settings;
            settings.remove("username");
            settings.remove("password");
        }

        auth_manager.SetCurrentUser(username);

        accept();

    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误！");
    }

}

bool LoginDialog::ValidateInput() {
    QString username = login_w_ui_->UserName_lineEdit->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入用户名！");
        return false;
    }

    if (login_w_ui_->Password_lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入密码！");
        login_w_ui_->Password_lineEdit->setFocus();
        return false;
    }

    if (!utils::AuthMangerGetInstance().UserExists(username)) {
        QMessageBox::warning(this, "错误", "用户不存在！");
        login_w_ui_->UserName_lineEdit->setFocus();
        login_w_ui_->UserName_lineEdit->selectAll();
        return false;
    }
    return true;
}

void LoginDialog::OnRegisterClicked() {
    RegisterDialog register_dialog(this);

    // 连接注册成功信号，自动填充用户名
//    connect(&register_dialog, &RegisterDialog::UserRegistered, [this](const QString& username) {
//        login_w_ui_->UserName_lineEdit->setText(username);
//    });

    if (register_dialog.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "提示", "注册成功！请使用新用户名和密码登录。");
        login_w_ui_->Password_lineEdit->setFocus();
    }
}

void LoginDialog::OnForgotPasswordClicked() {
    ForgotPasswordDialog forgot_password_dialog(this);

    // 预填当前输入的用户名（如果有）
    QString current_username = login_w_ui_->UserName_lineEdit->text().trimmed();
    if (!current_username.isEmpty()) {
        forgot_password_dialog.SetUsername(current_username);
    }

    if (forgot_password_dialog.exec() == QDialog::Accepted) {
        // 密码重置成功
        QMessageBox::information(this, "提示", "密码重置成功！请使用新密码登录。");

        // 清空密码框，让用户重新输入
        login_w_ui_->Password_lineEdit->clear();
        login_w_ui_->UserName_lineEdit->setFocus();
    }

}

void LoginDialog::UpdateLoginButtonState() {
    bool hasUsername = (!login_w_ui_->UserName_lineEdit->text().trimmed().isEmpty());
    bool hasPassword = (!login_w_ui_->Password_lineEdit->text().isEmpty());

    login_w_ui_->Login_pushButton->setEnabled(hasUsername && hasPassword);
}

void LoginDialog::LoadSavedCredentials() {
    QSettings settings;
    QString savedUsername = settings.value("username").toString();
    QString savedPassword = settings.value("password").toString();

    if (!savedUsername.isEmpty()) {
        login_w_ui_->UserName_lineEdit->setText(savedUsername);
        login_w_ui_->Remember_checkBox->setChecked(true);

        if (!savedPassword.isEmpty()) {
            login_w_ui_->Password_lineEdit->setText(savedPassword);
            // 自动聚焦登录按钮
            login_w_ui_->Login_pushButton->setFocus();
        }
    } else {
        /* do nothing */
    }
}

void LoginDialog::SaveCredentials() {
    QSettings settings;
    settings.setValue("username", login_w_ui_->UserName_lineEdit->text().trimmed());
    settings.setValue("password", login_w_ui_->Password_lineEdit->text());
}




