#include "logindialog.h"
#include "./ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , login_w_ui_(std::make_unique<Ui::LoginDialog>())
{
    login_w_ui_->setupUi(this);

    setWindowTitle("LoreKeeper - Login");

    // 设置密码框的回显模式
    login_w_ui_->Password_lineEdit->setEchoMode(QLineEdit::Password);

    SetupConnections();
    LoadSavedCredentials();
    UpdateLoginButtonState();
}

LoginDialog::~LoginDialog() = default;

void LoginDialog::SetupConnections() {
    // 连接登录按钮
    connect(login_w_ui_->Login_pushButton, &QPushButton::clicked, this, &LoginDialog::OnLoginClicked);
    // 连接注册按钮
    connect(login_w_ui_->Gegister_pushButton, &QPushButton::clicked, this, &LoginDialog::OnRegisterClicked);
    // @TODO 忘记密码按钮
    connect(login_w_ui_->Forgot_pushButton, &QPushButton::clicked, this, &LoginDialog::OnForgotPasswordClicked);
    // 实时验证输入
    connect(login_w_ui_->UserName_lineEdit, &QLineEdit::textChanged, this, &LoginDialog::UpdateLoginButtonState);
    connect(login_w_ui_->Password_lineEdit, &QLineEdit::textChanged, this, &LoginDialog::UpdateLoginButtonState);
}

void LoginDialog::OnLoginClicked() {
    if (!ValidateInput()) {
        return;
    }

    QString username = login_w_ui_->UserName_lineEdit->text().trimmed();
    QString password = login_w_ui_->Password_lineEdit->text();

    core::AuthManager& auth_manager = core::AuthMangerGetInstance();

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
    return true;
}

void LoginDialog::OnRegisterClicked() {
    QMessageBox::information(this, "提示", "注册功能尚未实现！");
}

void LoginDialog::OnForgotPasswordClicked() {
    QMessageBox::information(this, "提示", "忘记密码功能尚未实现！");
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




