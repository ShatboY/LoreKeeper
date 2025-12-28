#pragma once

#include "../BaseManager/BaseManager.h"
#include "../../../core/CardSystem/Card.h"
#include <QMap>
#include <QPixmap>
#include <QIcon>

namespace utils {

class ResourceManager : public BaseManager {
    Q_OBJECT

public:
    explicit ResourceManager(QObject *parent = nullptr);

    QPixmap getCardImage(int32_t cardId, bool useCache = true);
    QPixmap getCardBackImage();
    QIcon getClassIcon(core::Card::CardClass cardClass);
    QIcon getRarityIcon(core::Card::Rarity rarity);

    bool preloadImages(const QVector<int32_t> &cardIds);
    void clearImageCache();

private:
    // 资源缓存
    QMap<int32_t, QPixmap> cardImageCache_;
    QMap<core::Card::CardClass, QIcon> classIconCache_;
    QMap<core::Card::Rarity, QIcon> rarityIconCache_;

    // 默认图片
    QPixmap defaultCardImage_;
    QPixmap cardBackImage_;
}
}