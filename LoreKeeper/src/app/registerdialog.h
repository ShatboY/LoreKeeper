#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "ui/ui_registerdialog.h"
#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

    signals:
        void UserRegistered(const QString& username);

private slots:
    void OnRegisterClicked();
    void OnCancelClicked();
    void UpdateRegisterButtonState();

private:
    bool ValidInput();
    bool CheckUsernameAvailability(const QString& username);

private:
    std::unique_ptr<Ui::RegisterDialog> register_w_ui_{nullptr};
};

#endif // REGISTERDIALOG_H
