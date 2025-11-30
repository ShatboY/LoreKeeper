#include "forgotpassworddialog.h"

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgotPasswordDialog)
{
    ui->setupUi(this);
}

ForgotPasswordDialog::~ForgotPasswordDialog()
{
    delete ui;
}
