//
// Created by bobobo on 2025/12/13.
//

#ifndef LOREKEEPER_GAMEENGINE_H
#define LOREKEEPER_GAMEENGINE_H

#include <QObject>
#include <QTimer>
#include <QStack>
#include "../CardSystem/Card.h"
#include "../PlayerSystem/Player.h"

namespace core {

class GameEngine : public QObject {
    Q_OBJECT

public:
    enum GameState : int32_t {
        GAME_INIT,
        PLAYER_TURN,
        OPPONENT_TURN,
        GAME_END
    };

    static GameEngine& instance() noexcept;

    ~GameEngine() = default;

    void startNewGame();
    void endTurn();
    void playCard(int cardId, int slotIndex);
    void attack(int attackerSlot, int targetSlot)

    // 获取游戏状态
    GameState currentState() const;
    Player* currentPlayer() const;
    Player* opponent() const;

signals:
    void gameStateChanged(GameState state);
    void playerInfoUpdated();
    void opponentInfoUpdated();
    void cardPlayed(int cardId);
    void damageDealt(int source, int target, int amount);
    void gameLogAdded(const QString& log);
    void gameEnded(bool playerWon);

private:
    explicit GameEngine(QObject* parent = nullptr);
    GameEngine(const GameEngine&) = delete;
    GameEngine(GameEngine&&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;
    GameEngine& operator=(GameEngine&&) = delete;

    void setupPlayers();
    void drawInitialCards();
    void processAITurn();
    void checkGameEnd();

private:
    GameState m_gameState;
    Player* m_player;
    Player* m_opponent;
    QTimer* m_turnTimer;
    QStack<QString> m_gameHistory;
    int32_t m_turnNumber;

};

static GameEngine& GameEngineGetInstance() noexcept {
    return GameEngine::instance();
}

}
#endif //LOREKEEPER_GAMEENGINE_H
