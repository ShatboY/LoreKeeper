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
}

RegisterDialog::~RegisterDialog() = default;
