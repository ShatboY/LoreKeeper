#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H

#include "ui/ui_forgotpassworddialog.h"
#include <QDialog>
#include "../core/AuthManager/AuthManager.h"
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class ForgotPasswordDialog;
}

class ForgotPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPasswordDialog(QWidget *parent = nullptr);
    
    ForgotPasswordDialog(const ForgotPasswordDialog&) = delete;

    ForgotPasswordDialog& operator=(const ForgotPasswordDialog&) = delete;

    ForgotPasswordDialog(ForgotPasswordDialog&&) = delete;

    ForgotPasswordDialog& operator=(ForgotPasswordDialog&&) = delete;
    
    ~ForgotPasswordDialog() = default;

private slots:
    void OnResetPasswordClicked();
    void OnCancelClicked() noexcept;
    void UpdateResetPasswordButtonState() const;

private:
    std::unique_ptr<Ui::ForgotPasswordDialog> forgotpassword_w_ui_{nullptr};
};

#endif // FORGOTPASSWORDDIALOG_H
