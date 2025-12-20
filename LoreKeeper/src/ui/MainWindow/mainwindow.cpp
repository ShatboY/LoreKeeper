#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), main_w_ui_(std::make_unique<Ui::MainWindow>()) {
    main_w_ui_->setupUi(this);

    // 初始化游戏引擎
    game_engine_ = std::make_unique<core::GameEngine>();

    SetupUI();

    // 设置连接
    utils::ConnectManager::SetupConnections(this, main_w_ui_.get());
}

void MainWindow::SetupUI() {
    // 设置窗口标题
    setWindowTitle("LoreKeeper - Game");

    // 设置状态栏
    statusBar()->showMessage("欢迎来到LoreKeeper！");

    // 这里可以添加更多UI初始化代码
}

void MainWindow::OnLogout() {
    int32_t result = QMessageBox::question(this, "提示", "确定要退出吗？", QMessageBox::Yes | QMessageBox::No);

    if (QMessageBox::Yes == result) {
        utils::AuthMangerGetInstance().ClearCurrentUser();
        close();
    }
}
