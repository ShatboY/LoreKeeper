//
// Created by bobobo on 2025/12/13.
//
#pragma once

#ifndef LOREKEEPER_GAMEENGINE_H
#define LOREKEEPER_GAMEENGINE_H

#include <QObject>
#include <QTimer>
#include <QStack>
#include <QMap>
#include "../PlayerSystem/Player.h"
#include "../CardSystem/Card.h"
#include "../../utils/ConnectManager/ConnectManager.h"
#include "Deck.h"

namespace core {

class GameEngine : public QObject {
    Q_OBJECT

public:
    // 游戏状态
    enum GameState : int32_t {
        GAME_INIT = 0,       // 游戏初始化
        PLAYER_TURN_START,   // 玩家回合开始阶段
        PLAYER_TURN_MAIN,    // 玩家回合主阶段
        PLAYER_TURN_END,     // 玩家回合结束阶段
        OPPONENT_TURN_START, // 对手回合开始阶段
        OPPONENT_TURN_MAIN,  // 对手回合主阶段
        OPPONENT_TURN_END,   // 对手回合结束阶段
        GAME_END_WIN,        // 游戏结束-玩家胜利
        GAME_END_LOSE,       // 游戏结束-玩家失败
        GAME_END_DRAW        // 游戏结束-平局
    };

    // 游戏阶段
    enum GamePhase : int32_t {
        PHASE_BEGIN = 0, // 回合开始阶段
        PHASE_MAIN,      // 回合主阶段
        PHASE_BATTLE,    // 战斗阶段
        PHASE_END        // 回合结束阶段
    };

    struct GameConfig {
        int32_t startingHealth = 20;  // 玩家初始生命值
        int32_t startingMana = 0;     // 玩家初始法力值
        int32_t maxMana = 10;         // 最大法力值上限
        int32_t cardsPerTurn = 1;     // 每回合抽牌数量
        int32_t maxHandSize = 10;     // 手牌最大数量
        int32_t turnTimeLimit = 60;   // 回合时间限制(seconds)
        int32_t startingHandSize = 3; // 游戏开始时的手牌数量
    };

    static GameEngine &instance() noexcept;

    ~GameEngine() = default;

    // 游戏控制
    void startNewGame(const QString &playerName = "玩家");
    void restartGame();
    void surrender();

    // 回合操作
    void endTurn();
    void playCard(int32_t cardId, int32_t slotIndex = -1);
    void attack(int32_t attackerSlot, int32_t targetSlot);
    void useCardAbility(int32_t cardId);
    void drawCard();

    // 查询接口
    GameState currentState() const {
        return m_gameState;
    }
    GamePhase currentPhase() const {
        return m_currentPhase;
    }
    Player *currentPlayer() const {
        return m_currentPlayer;
    }
    Player *player() const {
        return m_player;
    }
    Player *opponent() const {
        return m_opponent;
    }
    int32_t turnNumber() const {
        return m_turnNumber;
    }
    int32_t timeRemaining() const {
        return m_timeRemaining;
    }

    // 验证接口
    bool canPlayCard(int32_t cardId, int32_t slotIndex = -1) const;
    bool canAttack(int32_t attackerSlot, int32_t targetSlot) const;
    bool canEndTurn() const;

    // 数据获取
    QVector<Card *> playerHand() const;
    QVector<Card *> opponentHand() const;
    QVector<Card *> playerField() const;
    QVector<Card *> opponentField() const;
    QStringList gameHistory() const;

signals:
    // 游戏状态变化
    void gameStateChanged(GameState newState);
    void gamePhaseChanged(GamePhase newPhase);
    void turnChanged(int32_t turnNumber);

    // 玩家信息更新
    void playerHealthChanged(int32_t health, int32_t maxHealth);
    void playerManaChanged(int32_t mana, int32_t maxMana);
    void opponentHealthChanged(int32_t health, int32_t maxHealth);

    // 卡牌相关
    void playerHandChanged();
    void opponentHandChanged();
    void playerFieldChanged(int32_t slotIndex, Card *card);
    void opponentFieldChanged(int32_t slotIndex, Card *card);
    void cardMovedToGraveyard(Card *card, bool isPlayerCard);

    // 游戏事件
    void damageDealt(const std::shared_ptr<Card> &source, const std::shared_ptr<Card> &target, const int32_t amount);
    void cardPlayed(Card *card, bool isPlayerCard);
    void attackPerformed(Card *attacker, Card *target);
    void gameLogAdded(const QString &log, bool isImportant = false);
    void gameEnded(bool playerWon, const QString &reason);

    // UI更新
    void updateDeckInfo(int32_t playerDeckSize, int32_t opponentDeckSize);
    void updateGraveyardInfo(int32_t playerGraveyardSize, int32_t opponentGraveyardSize);
    void updateTurnTimer(int32_t secondsRemaining);

private:
    explicit GameEngine(QObject *parent = nullptr);
    GameEngine(const GameEngine &) = delete;
    GameEngine(GameEngine &&) = delete;
    GameEngine &operator=(const GameEngine &) = delete;
    GameEngine &operator=(GameEngine &&) = delete;

    // 初始化
    void initializeGame();
    void setupDecks();
    void shuffleDecks();
    void drawInitialHands();

    // 回合管理
    void startTurn(std::unique_ptr<Player> &player);
    void endCurrentTurn();
    void nextPhase();
    void processBeginPhase();
    void processMainPhase();
    void processBattlePhase();
    void processEndPhase();

    // AI控制
    void processAITurn();
    void aiPlayCards();
    void aiAttack();

    // 游戏逻辑
    void resolveAttack(const std::shared_ptr<Card> &attacker, const std::shared_ptr<Card> &defender);
    void resolveDamage(Card *source, Card *target, int32_t damage);
    void checkDeath();
    void checkGameEnd();
    void triggerCardEffects(Card *card, const QString &event);

    // 辅助函数
    void addGameLog(const QString &message, bool isImportant = false);
    void updateAllUI();

private:
    // 游戏状态
    GameState gameState_;
    GamePhase currentPhase_;
    GameConfig config_;

    // 游戏对象
    std::unique_ptr<Player> player_;
    std::unique_ptr<Player> opponent_;
    Player *currentPlayer_;

    // 时间管理
    std::unique_ptr<QTimer> turnTimer_;
    std::unique_ptr<QTimer> aiTimer_;
    int32_t timeRemaining_;
    int32_t turnNumber_;

    // 游戏记录
    QStack<QString> actionHistory_;
    QStringList gameLog_;

    // 临时数据
    bool isProcessing_;
    QMap<int32_t, QVector<CardEffect>> triggeredEffects_;
};

static GameEngine &GameEngineGetInstance() noexcept {
    return GameEngine::instance();
}

} // namespace core
#endif // LOREKEEPER_GAMEENGINE_H
