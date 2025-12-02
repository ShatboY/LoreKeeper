#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "ui/ui_registerdialog.h"
#include <QDialog>
#include "../core/AuthManager/AuthManager.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

    RegisterDialog(const RegisterDialog&) = delete;

    RegisterDialog& operator=(const RegisterDialog&) = delete;

    RegisterDialog(RegisterDialog&&) = delete;

    RegisterDialog& operator=(RegisterDialog&&) = delete;

    ~RegisterDialog() = default;

    signals:
        void UserRegistered(const QString& username);

private slots:
    void OnRegisterClicked();
    void OnCancelClicked() noexcept;
    void UpdateRegisterButtonState() const;

private:
    bool ValidInput();
    static bool CheckUsernameAvailability(const QString& username);

private:
    std::unique_ptr<Ui::RegisterDialog> register_w_ui_{nullptr};
};

#endif // REGISTERDIALOG_H
