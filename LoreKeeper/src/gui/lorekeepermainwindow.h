#ifndef LOREKEEPERMAINWINDOW_H
#define LOREKEEPERMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoreKeeperMainWindow;
}
QT_END_NAMESPACE

class LoreKeeperMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoreKeeperMainWindow(QWidget *parent = nullptr);
    ~LoreKeeperMainWindow();

private:
    Ui::LoreKeeperMainWindow *ui;
};
#endif // LOREKEEPERMAINWINDOW_H
