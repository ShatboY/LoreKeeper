#include "registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , register_w_ui_(std::make_unique<Ui::RegisterDialog>()) {
    register_w_ui_->setupUi(this);

    setWindowTitle("LoreKeeper - 用户注册");
    register_w_ui_->Password_lineEdit->setEchoMode(QLineEdit::Password);
    register_w_ui_->ConfirmPassword_lineEdit->setEchoMode(QLineEdit::Password);

    // 连接信号槽
    connect(register_w_ui_->Register_pushButton, &QPushButton::clicked, this, &RegisterDialog::OnRegisterClicked);
    connect(register_w_ui_->Cancel_pushButton, &QPushButton::clicked, this, &RegisterDialog::OnCancelClicked);

    // 实时验证输入
    connect(register_w_ui_->UserName_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::UpdateRegisterButtonState);
    connect(register_w_ui_->Password_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::UpdateRegisterButtonState);
    connect(register_w_ui_->ConfirmPassword_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::UpdateRegisterButtonState);

    UpdateRegisterButtonState();
}

void RegisterDialog::OnRegisterClicked() {
    if (!ValidInput()) {
        return;
    }

    QString username = register_w_ui_->UserName_lineEdit->text().trimmed();
    QString password = register_w_ui_->Password_lineEdit->text();

    // 注册新用户
    if (core::AuthMangerGetInstance().RegisterUser(username, password)) {
        QMessageBox::information(this, "注册成功", QString("用户 %1 注册成功！").arg(username));

        // 发射注册成功信号
        emit UserRegistered(username);

        accept();
    } else {
        QMessageBox::warning(this, "注册失败", QString("用户 %1 注册失败！请重试").arg(username));
    }
}

void RegisterDialog::OnCancelClicked() noexcept{
    reject();
}

void RegisterDialog::UpdateRegisterButtonState() const {
    QString username = register_w_ui_->UserName_lineEdit->text().trimmed();
    QString password = register_w_ui_->Password_lineEdit->text();
    QString confirmPassword = register_w_ui_->ConfirmPassword_lineEdit->text();

    register_w_ui_->Register_pushButton->setEnabled(
            (!username.isEmpty()) && (!password.isEmpty()) && (!confirmPassword.isEmpty()));

    register_w_ui_->Register_pushButton->setDefault(true);
}

bool RegisterDialog::ValidInput() {
    QString username = register_w_ui_->UserName_lineEdit->text().trimmed();
    QString password = register_w_ui_->Password_lineEdit->text();
    QString confirmPassword = register_w_ui_->ConfirmPassword_lineEdit->text();

    // 验证用户名
    if (username.isEmpty()) {
        QMessageBox::warning(this, "用户名不能为空", "请输入用户名");
        register_w_ui_->UserName_lineEdit->setFocus();
        return false;
    }

    // 验证用户名长度
    if (username.length() < 5U) {
        QMessageBox::warning(this, "用户名长度太短", "用户名长度不能小于5个字符");
        register_w_ui_->UserName_lineEdit->setFocus();
        register_w_ui_->UserName_lineEdit->selectAll();
        return false;
    }

    // 验证用户名重复
    if (CheckUsernameAvailability(username)) {
        QMessageBox::warning(this, "用户名已存在", "请选择其他用户名");
        register_w_ui_->UserName_lineEdit->setFocus();
        register_w_ui_->UserName_lineEdit->selectAll();
        return false;
    }

    // 验证用户名格式
    static const QRegularExpression username_regex("^[a-zA-Z0-9_]+$");
    if  (!username_regex.match(username).hasMatch()) {
        QMessageBox::warning(this, "用户名格式错误", "用户名只能包含字母、数字和下划线");
        register_w_ui_->UserName_lineEdit->setFocus();
        register_w_ui_->UserName_lineEdit->selectAll();
        return false;
    }

    // 验证密码
    if (password.isEmpty()) {
        QMessageBox::warning(this, "密码不能为空", "请输入密码");
        register_w_ui_->Password_lineEdit->setFocus();
        return false;
    }

    if (password.length() < 8U) {
        QMessageBox::warning(this, "密码长度太短", "密码长度不能小于8个字符");
        register_w_ui_->Password_lineEdit->setFocus();
        register_w_ui_->Password_lineEdit->selectAll();
        return false;
    }

    // 验证确认密码
    if (confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "确认密码不能为空", "请输入确认密码");
        register_w_ui_->ConfirmPassword_lineEdit->setFocus();
        return false;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "密码不一致", "请输入一致的密码");
        register_w_ui_->ConfirmPassword_lineEdit->setFocus();
        register_w_ui_->ConfirmPassword_lineEdit->selectAll();
        return false;
    }

    return true;
}

bool RegisterDialog::CheckUsernameAvailability(const QString& username) {
    return core::AuthMangerGetInstance().UserExists(username);
}













