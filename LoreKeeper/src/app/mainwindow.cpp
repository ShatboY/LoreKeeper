#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , main_w_ui_(std::make_unique<Ui::MainWindow>())
{
    main_w_ui_->setupUi(this);

    SetupUI();
    SetupConnections();
    UpdateWelcomeMessage();
}

void MainWindow::SetupUI() {
    // 设置窗口标题
    setWindowTitle("LoreKeeper - 个人资料");

    // 设置状态栏
    statusBar()->showMessage("欢迎使用LoreKeeper！");

    // 这里可以添加更多UI初始化代码

}

void MainWindow::SetupConnections() {
    // 这里可以添加更多信号槽连接

    // 添加退出逻辑

}

void MainWindow::OnLogout() {
    int32_t result = QMessageBox::question(this, "提示", "确定要退出吗？", QMessageBox::Yes | QMessageBox::No);

    if (QMessageBox::Yes == result) {
        core::AuthMangerGetInstance().ClearCurrentUser();
        close();
    }
}

void MainWindow::UpdateWelcomeMessage() {
    QString username = core::AuthMangerGetInstance().GetCurrentUser();
    QString welcome_message = QString("欢迎， %1！").arg(username);

    if (main_w_ui_->Welcome_label) {
        main_w_ui_->Welcome_label->setText(welcome_message);
    }

    setWindowTitle(QString("LoreKeeper - %1").arg(username));
    statusBar()->showMessage(QString("已登录，当前用户：%1").arg(username));
}



