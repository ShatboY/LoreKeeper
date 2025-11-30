#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H

#include "ui/ui_forgotpassworddialog.h"
#include <QDialog>

namespace Ui {
class ForgotPasswordDialog;
}

class ForgotPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPasswordDialog(QWidget *parent = nullptr);
    ~ForgotPasswordDialog();

private:
    Ui::ForgotPasswordDialog *ui;
};

#endif // FORGOTPASSWORDDIALOG_H
