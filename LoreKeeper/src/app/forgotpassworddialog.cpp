#include "forgotpassworddialog.h"

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , forgotpassword_w_ui_(std::make_unique<Ui::ForgotPasswordDialog>())
{
    forgotpassword_w_ui_->setupUi(this);

    setWindowTitle("LoreKeeper - 忘记密码");
    forgotpassword_w_ui_->NewPassword_lineEdit->setEchoMode(QLineEdit::Password);
    forgotpassword_w_ui_->ConfirmPassword_lineEdit->setEchoMode(QLineEdit::Password);

    // 连接信号槽
    connect(forgotpassword_w_ui_->ResetPassword_pushButton, &QPushButton::clicked, this, &ForgotPasswordDialog::OnResetPasswordClicked);

    connect(forgotpassword_w_ui_->Cancel_pushButton, &QPushButton::clicked, this, &ForgotPasswordDialog::OnCancelClicked);

    connect(forgotpassword_w_ui_->UserName_lineEdit, &QLineEdit::textChanged, this, &ForgotPasswordDialog::UpdateResetPasswordButtonState);
    connect(forgotpassword_w_ui_->NewPassword_lineEdit, &QLineEdit::textChanged, this, &ForgotPasswordDialog::UpdateResetPasswordButtonState);
    connect(forgotpassword_w_ui_->ConfirmPassword_lineEdit, &QLineEdit::textChanged, this, &ForgotPasswordDialog::UpdateResetPasswordButtonState);

    UpdateResetPasswordButtonState();
}

void ForgotPasswordDialog::OnResetPasswordClicked() {

}

void ForgotPasswordDialog::OnCancelClicked() noexcept {
    reject();
}

void ForgotPasswordDialog::UpdateResetPasswordButtonState() const {

}