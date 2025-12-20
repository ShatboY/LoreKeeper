#include "../ui/LoginWindow/logindialog.h"
#include "../ui/MainWindow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QApplication::setApplicationName("LoreKeeper");
    QApplication::setApplicationVersion("0.0.1");
    QApplication::setOrganizationName("LoreKeeper");

    LoginDialog login_window;

    // 登录成功，显示主窗口
    if (login_window.exec() == QDialog::Accepted) {
        MainWindow main_window;
        main_window.show();
        return QApplication::exec();
    }

    return 0;
}
