//
// Created by bobobo on 2025/12/4.
//
#pragma once

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
#include "../../core/GameEngine/GameEngine.h"

namespace utils {

class ConnectManager {
public:
    // 统一的连接设置接口
    template <typename DialogType, typename UiClass>
    static void SetupConnections(DialogType *dialog, UiClass *ui) {
        SetupDialogConnections(*dialog, *ui);
    }
    // 重载版本，用于不需要UI参数的对象
    template <typename ClassType>
    static void SetupConnections(ClassType *class) {
        SetupDialogConnections(*class);
    }

private:
    template <typename DialogType, typename UiClass>
    static void SetupDialogConnections(DialogType &dialog, UiClass &ui);

    template <typename ClassType>
    staticvoid SetupDialogConnections(ClassType &class);
};

} // namespace utils

#endif // LOREKEEPER_CONNECTMANAGER_H
