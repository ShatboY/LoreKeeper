#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include "../../utils/AuthManager/AuthManager.h"
#include "../../utils/ConnectManager/ConnectManager.h"
#include "../../core/GameEngine/GameEngine.h"
#include <QMessageBox>
#include <QStatusBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

public:
    std::unique_ptr<core::GameEngine> game_engine_{nullptr};

private slots:
    void OnLogout();

private:
    void SetupUI();

private:
    std::unique_ptr<Ui::MainWindow> main_w_ui_{nullptr};
};

#endif // MAINWINDOW_H
