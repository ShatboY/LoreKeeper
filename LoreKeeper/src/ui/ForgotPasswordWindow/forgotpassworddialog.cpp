#include "forgotpassworddialog.h"

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent) : QDialog(parent), forgotpassword_w_ui_(std::make_unique<Ui::ForgotPasswordDialog>()) {
    forgotpassword_w_ui_->setupUi(this);

    setWindowTitle("LoreKeeper - 忘记密码");
    forgotpassword_w_ui_->NewPassword_lineEdit->setEchoMode(QLineEdit::Password);
    forgotpassword_w_ui_->ConfirmPassword_lineEdit->setEchoMode(QLineEdit::Password);

    // 设置连接
    utils::ConnectManager::SetupConnections(this, forgotpassword_w_ui_.get());
}

void ForgotPasswordDialog::OnResetPasswordClicked() {
    if (!ValidateInput()) {
        return;
    }

    QString username = forgotpassword_w_ui_->UserName_lineEdit->text().trimmed();
    QString new_password = forgotpassword_w_ui_->NewPassword_lineEdit->text();

    // 重置密码
    if (utils::AuthMangerGetInstance().ChangePassword(username, new_password)) {
        QMessageBox::information(this, "提示", QString("用户 %1 密码重置成功").arg(username));
        accept();
    } else {
        QMessageBox::warning(this, "警告", QString("用户 %1 密码重置失败，请重试！").arg(username));
    }
}

bool ForgotPasswordDialog::ValidateInput() {
    QString username = forgotpassword_w_ui_->UserName_lineEdit->text().trimmed();
    QString new_password = forgotpassword_w_ui_->NewPassword_lineEdit->text();
    QString confirm_password = forgotpassword_w_ui_->ConfirmPassword_lineEdit->text();

    // 验证用户名
    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名不能为空");
        forgotpassword_w_ui_->UserName_lineEdit->setFocus();
        return false;
    }

    // 验证用户名是否存在
    if (!CheckUserExistence(username)) {
        QMessageBox::warning(this, "警告", "用户名不存在");
        forgotpassword_w_ui_->UserName_lineEdit->setFocus();
        forgotpassword_w_ui_->UserName_lineEdit->selectAll();
        return false;
    }

    // 验证新密码
    if (new_password.isEmpty()) {
        QMessageBox::warning(this, "警告", "新密码不能为空");
        forgotpassword_w_ui_->NewPassword_lineEdit->setFocus();
        return false;
    }

    // 验证新密码长度
    if (new_password.length() < 8U) {
        QMessageBox::warning(this, "警告", "新密码长度不能小于8个字符");
        forgotpassword_w_ui_->NewPassword_lineEdit->setFocus();
        forgotpassword_w_ui_->NewPassword_lineEdit->selectAll();
        return false;
    }

    // 验证确认密码
    if (confirm_password.isEmpty()) {
        QMessageBox::warning(this, "警告", "确认密码不能为空");
        forgotpassword_w_ui_->ConfirmPassword_lineEdit->setFocus();
        return false;
    }

    // 验证新密码和确认密码一致
    if (new_password != confirm_password) {
        QMessageBox::warning(this, "警告", "新密码和确认密码不一致");
        forgotpassword_w_ui_->ConfirmPassword_lineEdit->setFocus();
        forgotpassword_w_ui_->ConfirmPassword_lineEdit->selectAll();
        return false;
    }
    return true;
}

void ForgotPasswordDialog::SetUsername(const QString &username) noexcept {
    forgotpassword_w_ui_->UserName_lineEdit->setText(username);
    forgotpassword_w_ui_->NewPassword_lineEdit->setFocus();
}

bool ForgotPasswordDialog::CheckUserExistence(const QString &username) {
    return utils::AuthMangerGetInstance().UserExists(username);
}

void ForgotPasswordDialog::OnCancelClicked() noexcept {
    reject();
}

void ForgotPasswordDialog::UpdateResetPasswordButtonState() const {
    QString username = forgotpassword_w_ui_->UserName_lineEdit->text().trimmed();
    QString new_password = forgotpassword_w_ui_->NewPassword_lineEdit->text();
    QString confirm_password = forgotpassword_w_ui_->ConfirmPassword_lineEdit->text();

    forgotpassword_w_ui_->ResetPassword_pushButton->setEnabled(
        (!username.isEmpty()) && (!new_password.isEmpty()) && (!confirm_password.isEmpty()));

    forgotpassword_w_ui_->ResetPassword_pushButton->setDefault(true);
}