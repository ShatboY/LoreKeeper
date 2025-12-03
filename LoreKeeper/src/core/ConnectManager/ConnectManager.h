//
// Created by bobobo on 2025/12/4.
//

#ifndef LOREKEEPER_CONNECTMANAGER_H
#define LOREKEEPER_CONNECTMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>


namespace core {

class ConnectManager {
public:
    // 统一的连接设置接口
    template<typename DialogType, typename UiClass>
    static void SetupConnections(DialogType* dialog, UiClass* ui) {
        SetupDialogConnections(dialog, ui);
    }

private:
    template<typename DialogType, typename UiClass>
    static void SetupDialogConnections(DialogType* dialog, UiClass* ui);
};

// 模板函数定义
template<typename DialogType, typename UiClass>
void ConnectManager::SetupDialogConnections(DialogType* dialog, UiClass* ui) {
    // 可以提供默认实现或保持为空，具体实现在.cpp中通过特化提供
}

} // core

#endif //LOREKEEPER_CONNECTMANAGER_H
