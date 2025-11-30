#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../core/AuthManager/AuthManager.h"
#include <QMessageBox>
#include <QStatusBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnLogout();
    void UpdateWelcomeMessage();

private:
    void SetupUI();
    void SetupConnections();

private:
    std::unique_ptr<Ui::MainWindow> main_w_ui_{nullptr};
};

#endif // MAINWINDOW_H
