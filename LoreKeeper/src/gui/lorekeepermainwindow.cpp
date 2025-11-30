#include "lorekeepermainwindow.h"
#include "./ui_lorekeepermainwindow.h"

LoreKeeperMainWindow::LoreKeeperMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoreKeeperMainWindow)
{
    ui->setupUi(this);
}

LoreKeeperMainWindow::~LoreKeeperMainWindow()
{
    delete ui;
}
