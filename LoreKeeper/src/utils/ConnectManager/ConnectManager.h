//
// Created by bobobo on 2025/12/4.
//

#ifndef LOREKEEPER_CONNECTMANAGER_H
#define LOREKEEPER_CONNECTMANAGER_H

#include <QObject>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>

#include "../../ui/LoginWindow/logindialog.h"
#include "../../ui/RegisterWindow/registerdialog.h"
#include "../../ui/ForgotPasswordWindow/forgotpassworddialog.h"
#include "../../ui/MainWindow/mainwindow.h"

namespace utils {

class ConnectManager {
public:
    // 统一的连接设置接口
    template<typename DialogType, typename UiClass>
    static void SetupConnections(DialogType* dialog, UiClass* ui) {
        SetupDialogConnections(*dialog, *ui);
    }

private:
    template<typename DialogType, typename UiClass>
    static void SetupDialogConnections(DialogType& dialog, UiClass& ui);
};

} // core

#endif //LOREKEEPER_CONNECTMANAGER_H
