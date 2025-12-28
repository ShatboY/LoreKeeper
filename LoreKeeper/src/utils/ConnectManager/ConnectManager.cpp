//
// Created by bobobo on 2025/12/4.
//

#include "ConnectManager.h"

namespace utils {
// 登录窗口连接
template <>
void ConnectManager::SetupDialogConnections<LoginDialog, Ui::LoginDialog>(LoginDialog &dialog, Ui::LoginDialog &ui) {
    QObject::connect(ui.Login_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnLoginClicked(); });
    // 连接注册按钮
    QObject::connect(ui.Gegister_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnRegisterClicked(); });
    // 忘记密码按钮
    QObject::connect(ui.Forgot_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnForgotPasswordClicked(); });
    // 实时验证输入
    QObject::connect(ui.UserName_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateLoginButtonState(); });
    QObject::connect(ui.Password_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateLoginButtonState(); });

    // 更新按钮状态
    dialog.UpdateLoginButtonState();
}

// 忘记密码窗口连接
template <>
void ConnectManager::SetupDialogConnections<ForgotPasswordDialog, Ui::ForgotPasswordDialog>(ForgotPasswordDialog &dialog, Ui::ForgotPasswordDialog &ui) {
    QObject::connect(ui.ResetPassword_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnResetPasswordClicked(); });

    QObject::connect(ui.Cancel_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnCancelClicked(); });

    QObject::connect(ui.UserName_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateResetPasswordButtonState(); });

    QObject::connect(ui.NewPassword_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateResetPasswordButtonState(); });
    QObject::connect(ui.ConfirmPassword_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateResetPasswordButtonState(); });

    // 更新按钮状态
    dialog.UpdateResetPasswordButtonState();
}

// 注册用户窗口连接
template <>
void ConnectManager::SetupDialogConnections<RegisterDialog, Ui::RegisterDialog>(RegisterDialog &dialog, Ui::RegisterDialog &ui) {
    QObject::connect(ui.Register_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnRegisterClicked(); });

    QObject::connect(ui.Cancel_pushButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.OnCancelClicked(); });

    QObject::connect(ui.UserName_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateRegisterButtonState(); });

    QObject::connect(ui.Password_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateRegisterButtonState(); });
    QObject::connect(ui.ConfirmPassword_lineEdit, &QLineEdit::textChanged, &dialog,
                     [&dialog]() { dialog.UpdateRegisterButtonState(); });

    // 更新按钮状态
    dialog.UpdateRegisterButtonState();
}

// 注册MainWindow连接
template <>
void ConnectManager::SetupDialogConnections<MainWindow, Ui::MainWindow>(MainWindow &dialog, Ui::MainWindow &ui) {
    // 按钮连接
    QObject::connect(ui.endTurnButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onEndTurnClicked(); });
    QObject::connect(ui.drawCardButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onDrawCardClicked(); });
    QObject::connect(ui.surrenderButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onSurrenderClicked(); });
    QObject::connect(ui.graveyardButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onViewGraveyardClicked(); });
    QObject::connect(ui.deckViewButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onViewDeckClicked(); });
    QObject::connect(ui.helpButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onHelpClicked(); });
    QObject::connect(ui.viewGraveyardButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onViewGraveyardClicked(); });
    QObject::connect(ui.viewDeckButton, &QPushButton::clicked, &dialog,
                     [&dialog]() { dialog.onViewDeckClicked(); });
    //  TODO  QObject::connect(ui.viewHistoryButton, &QPushButton::clicked, []() { Logger::showLogWindow(); });

    // 战场槽位连接
    for (int32_t i = 0; i < 7; ++i) {
        if (dialog.playerSlots_.contains(i)) {
            QObject::connect(dialog.playerSlots_[i], &ui::ClickableFrame::clicked,
                             [&dialog, i]() { dialog.onPlayerSlotClicked(i); });
        }

        if (dialog.opponentSlots_.contains(i)) {
            QObject::connect(dialog.opponentSlots_[i], &ui::ClickableFrame::clicked,
                             [&dialog, i]() { dialog.onOpponentSlotClicked(i); });
        }
    }

    // 游戏引擎信号连接
    QObject::connect(dialog.gameEngine_, &core::GameEngine::gameStateChanged,
                     &dialog, &MainWindow::onGameStateChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::gamePhaseChanged,
                     &dialog, &MainWindow::onGamePhaseChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::turnChanged,
                     &dialog, &MainWindow::onTurnChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::playerHealthChanged,
                     &dialog, &MainWindow::onPlayerHealthChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::playerManaChanged,
                     &dialog, &MainWindow::onPlayerManaChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::opponentHealthChanged,
                     &dialog, &MainWindow::onOpponentHealthChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::playerHandChanged,
                     &dialog, &MainWindow::onPlayerHandChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::opponentHandChanged,
                     &dialog, &MainWindow::onOpponentHandChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::playerFieldChanged,
                     &dialog, &MainWindow::onPlayerFieldChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::opponentFieldChanged,
                     &dialog, &MainWindow::onOpponentFieldChanged);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::cardMovedToGraveyard,
                     &dialog, &MainWindow::onCardMovedToGraveyard);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::damageDealt,
                     &dialog, &MainWindow::onDamageDealt);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::cardPlayed,
                     &dialog, &MainWindow::onCardPlayed);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::attackPerformed,
                     &dialog, &MainWindow::onAttackPerformed);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::gameLogAdded,
                     &dialog, &MainWindow::onGameLogAdded);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::gameEnded,
                     &dialog, &MainWindow::onGameEnded);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::updateDeckInfo,
                     &dialog, &MainWindow::onUpdateDeckInfo);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::updateGraveyardInfo,
                     &dialog, &MainWindow::onUpdateGraveyardInfo);
    QObject::connect(dialog.gameEngine_, &core::GameEngine::updateTurnTimer,
                     &dialog, &MainWindow::onUpdateTurnTimer);

    // 菜单栏
    QObject::connect(ui.actionNewGame, &QAction::triggered, &dialog, &MainWindow::onStartGame);
    QObject::connect(ui.actionRestart, &QAction::triggered, dialog.gameEngine_, &core::GameEngine::restartGame);
    QObject::connect(ui.actionSurrender, &QAction::triggered, &dialog, &MainWindow::onSurrenderClicked);
    QObject::connect(ui.actionExit, &QAction::triggered, &dialog, &QMainWindow::close);
    QObject::connect(ui.actionDeckEditor, &QAction::triggered, &dialog, []() { /* TODO: 打开牌库编辑器 */ });
    QObject::connect(ui.actionSettings, &QAction::triggered, &dialog, []() { /* TODO: 打开设置 */ });
    QObject::connect(ui.actionAbout, &QAction::triggered, &dialog, &MainWindow::showHelpDialog);
}

// 注册计时器相关连接
template <>
void ConnectManager::SetupDialogConnections<core::GameEngine>(core::GameEngine &gameEngine) {
    // 连接计时器信号
    QObject::connect(gameEngine.turnTimer_, &QTimer::timeout, &gameEngine, [&gameEngine]() {
        gameEngine.timeRemaining_--;
        emit gameEngine.updateTurnTimer(gameEngine.timeRemaining_);

        if (gameEngine.timeRemaining_ <= 0) {
            gameEngine.addGameLog("时间到，自动结束回合");
            gameEngine.endTurn();
        }
    });

    QObject::connect(gameEngine.aiTimer_, &QTimer::timeout, &gameEngine,
                     [&gameEngine]() {
                         gameEngine.processAITurn();
                     });
}
} // namespace utils
