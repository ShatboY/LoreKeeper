//
// Created by bobobo on 2025/12/13.
//

#ifndef LOREKEEPER_CARD_H
#define LOREKEEPER_CARD_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QPainter>
#include <QFile>
#include <QMap>

namespace core {

class Card : public QObject {
    Q_OBJECT

public:
    // 卡牌类型
    enum CardType : int32_t {
        TYPE_CREATURE = 0, // 生物卡牌
        TYPE_SPELL,        // 法术卡牌
        TYPE_ENCHANTMENT,  // 魔法增强卡牌
        TYPE_ARTIFACT,     // 神器卡牌
        TYPE_WEAPON        // 武器卡牌
    };
    // 稀有度
    enum Rarity : int32_t {
        RARITY_COMMON = 0, // 普通
        RARITY_UNCOMMON,   // 不常见
        RARITY_RARE,       // 稀有
        RARITY_EPIC,       // 史诗
        RARITY_LEGENDARY   // 传说
    };
    // 卡牌职业
    enum CardClass : int32_t {
        CLASS_NEUTRAL = 0, // 中立职业
        CLASS_MAGE,        // 法师职业
        CLASS_WARRIOR,     // 战士职业
        CLASS_PRIEST,      // 牧师职业
        CLASS_ROGUE,       // 潜行者职业
        CLASS_HUNTER,      // 猎人职业
        CLASS_WARLOCK,     // 术士职业
        CLASS_DRUID,       // 德鲁伊职业
        CLASS_SHAMAN,      // 萨满职业
        CLASS_PALADIN      // 圣骑士职业
    };

    struct CardData {
        int32_t id;
        QString name;
        QString description;
        CardType type;
        Rarity rarity;
        CardClass cardClass;
        int32_t manaCost;
        int32_t attack;
        int32_t health;
        QVector<QString> keywords;
        QString effectScript;
        QString imagePath;
    };

    Card(const CardData &data, QObject *parent = nullptr);
    ~Card() = default;

    // 基本信息
    int32_t id() const {
        return m_data.id;
    }
    QString name() const {
        return m_data.name;
    }
    QString description() const {
        return m_data.description;
    }
    CardType type() const {
        return m_data.type;
    }
    Rarity rarity() const {
        return m_data.rarity;
    }
    CardClass cardClass() const {
        return m_data.cardClass;
    }
    int32_t manaCost() const {
        return m_data.manaCost;
    }

    // 生物属性
    bool isCreature() const {
        return m_data.type == TYPE_CREATURE;
    }
    int32_t attack() const {
        return m_data.attack;
    }
    int32_t health() const {
        return m_data.health;
    }
    int32_t currentHealth() const {
        return m_currentHealth;
    }

    // 关键词检查
    bool hasKeyword(const QString &keyword) const;
    bool hasTaunt() const {
        return hasKeyword("taunt");
    }
    bool hasCharge() const {
        return hasKeyword("charge");
    }
    bool hasDivineShield() const {
        return hasKeyword("divine_shield");
    }
    bool hasWindfury() const {
        return hasKeyword("windfury");
    }
    bool hasStealth() const {
        return hasKeyword("stealth");
    }
    bool hasPoisonous() const {
        return hasKeyword("poisonous");
    }
    bool hasLifesteal() const {
        return hasKeyword("lifesteal");
    }
    bool hasRush() const {
        return hasKeyword("rush");
    }

    // 状态
    bool isExhausted() const {
        return m_isExhausted;
    }
    void setExhausted(bool exhausted) {
        m_isExhausted = exhausted;
    }
    bool canAttack() const;
    bool canAttackImmediately() const;

    // 视觉效果
    QPixmap cardImage() const;
    QString cardFrameColor() const;
    QString rarityColor() const;
    QString classColor() const;

    // 战斗操作
    void takeDamage(int32_t damage);
    void heal(int32_t amount);
    void buff(int32_t attackBonus, int32_t healthBonus);
    void addKeyword(const QString &keyword);
    void removeKeyword(const QString &keyword);

    // 效果系统
    void triggerEffect(const QString &event, Card *source = nullptr, Card *target = nullptr);
    void addEffect(CardEffect *effect);
    void removeEffect(CardEffect *effect);

    // 临时属性（用于回合内效果）
    void setTemporaryAttack(int32_t attack) {
        m_temporaryAttack = attack;
    }
    void setTemporaryHealth(int32_t health) {
        m_temporaryHealth = health;
    }

signals:
    void cardPlayed();
    void cardAttacked(Card *target);
    void cardDamaged(int32_t damage);
    void cardHealed(int32_t amount);
    void cardDestroyed();
    void cardBuffed(int32_t attack, int32_t health);
    void keywordAdded(const QString &keyword);
    void keywordRemoved(const QString &keyword);

protected:
    CardData cardData_;

    // 动态状态
    int32_t currentHealth_;
    int32_t currentAttack_; // 考虑buff
    bool isExhausted_;
    bool hasDivineShield_;
    bool isStealthed_;

    // 临时状态
    int32_t temporaryAttack_;
    int32_t temporaryHealth_;

    // 效果
    QVector<std::unique_ptr<CardEffect>> cardEffects_;

    // 关键词集合（用于快速查找）
    QSet<QString> keywordSet_;
};

} // namespace core

#endif // LOREKEEPER_CARD_H
