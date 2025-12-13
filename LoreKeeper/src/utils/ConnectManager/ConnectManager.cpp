//
// Created by bobobo on 2025/12/4.
//

#include "ConnectManager.h"

namespace utils {
// 登录窗口连接
template<>
void ConnectManager::SetupDialogConnections<LoginDialog, Ui::LoginDialog>(LoginDialog& dialog, Ui::LoginDialog& ui) {
    QObject::connect(ui.Login_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() {dialog.OnLoginClicked();});
    // 连接注册按钮
    QObject::connect(ui.Gegister_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog](){dialog.OnRegisterClicked();});
    // 忘记密码按钮
    QObject::connect(ui.Forgot_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog](){dialog.OnForgotPasswordClicked();});
    // 实时验证输入
    QObject::connect(ui.UserName_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateLoginButtonState();});
    QObject::connect(ui.Password_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateLoginButtonState();});

    // 更新按钮状态
    dialog.UpdateLoginButtonState();
}

// 忘记密码窗口连接
template<>
void ConnectManager::SetupDialogConnections<ForgotPasswordDialog, Ui::ForgotPasswordDialog>(ForgotPasswordDialog& dialog, Ui::ForgotPasswordDialog& ui) {

    QObject::connect(ui.ResetPassword_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() {dialog.OnResetPasswordClicked();});

    QObject::connect(ui.Cancel_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog](){dialog.OnCancelClicked();});

    QObject::connect(ui.UserName_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateResetPasswordButtonState();});

    QObject::connect(ui.NewPassword_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateResetPasswordButtonState();});
    QObject::connect(ui.ConfirmPassword_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateResetPasswordButtonState();});

    // 更新按钮状态
    dialog.UpdateResetPasswordButtonState();
}

// 注册用户窗口连接
template<>
void ConnectManager::SetupDialogConnections<RegisterDialog, Ui::RegisterDialog>(RegisterDialog& dialog, Ui::RegisterDialog& ui) {

    QObject::connect(ui.Register_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() {dialog.OnRegisterClicked();});

    QObject::connect(ui.Cancel_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog](){dialog.OnCancelClicked();});

    QObject::connect(ui.UserName_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateRegisterButtonState();});

    QObject::connect(ui.Password_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog](){dialog.UpdateRegisterButtonState();});
    QObject::connect(ui.ConfirmPassword_lineEdit, &QLineEdit::textChanged, &dialog,
                         [&dialog](){dialog.UpdateRegisterButtonState();});

    // 更新按钮状态
    dialog.UpdateRegisterButtonState();
}

// 注册MainWindow连接
template<>
void ConnectManager::SetupDialogConnections<MainWindow, Ui::MainWindow>(MainWindow& dialog, Ui::MainWindow& ui) {
    // 游戏引擎信号
    QObject::connect(dialog.game_engine_, &core::GameEngine::gameStateChanged, &dialog,
                     [&dialog]() {dialog.onGameStateChanged();});

    QObject::connect(dialog.game_engine_, &core::GameEngine::playerInfoUpdated, &dialog,
                     [&dialog]() {dialog.updatePlayerInfo();});

    QObject::connect(dialog.game_engine_, &core::GameEngine::opponentInfoUpdated, &dialog,
                     [&dialog]() {dialog.updateOpponentInfo();});

    QObject::connect(dialog.game_engine_, &core::GameEngine::gameLogAdded, &dialog,
                     [&dialog]() {dialog.addGameLog();});

    // 按钮连接
    QObject::connect(ui.endTurnButton, &QPushButton::clicked, &dialog,
                     [&dialog]() {dialog.game_engine_->endTurn();});

    QObject::connect(ui.drawCardButton, &QPushButton::clicked, &dialog,
                     [&dialog]() {dialog.onDrawCardClicked();});

    QObject::connect(ui.surrenderButton, &QPushButton::clicked, &dialog,
                     [&dialog]() {dialog.onSurrenderClicked();});

}

} // utils

