//
// Created by bobobo on 2025/12/13.
//

#include "GameEngine.h"

namespace core {

GameEngine::GameEngine(QObject *parent) :
    QObject(parent),
    gameState_(GAME_INIT),
    currentPhase_(PHASE_BEGIN),
    player_(nullptr),
    opponent_(nullptr),
    currentPlayer_(nullptr),
    timeRemaining_(0),
    turnNumber_(0),
    isProcessing_(false) {
    // 创建计时器
    turnTimer_ = std::make_unique<QTimer>();
    turnTimer_->setInterval(1000); // 1s
    aiTimer_ = std::make_unique<QTimer>();
    aiTimer_->setInterval(1500); // AI思考时间

    // 设置连接
    utils::ConnectManager::SetupConnections(this);
}

GameEngine &GameEngine::instance() noexcept {
    static GameEngine instance;
    return instance;
}

void GameEngine::startNewGame(const QString &playerName) {
    if (isProcessing_) {
        return;
    }

    // 清理旧游戏
    if (player_) {
        player_.reset();
    }
    if (opponent_) {
        opponent_.reset();
    }

    // 创建玩家
    player_ = std::make_unique<Player>(playerName);
    player_->setHealth(config_.startingHealth);
    player_->setMaxHealth(config_.startingHealth);
    player_->setMana(0);
    player_->setMaxMana(config_.startingMana);

    // 创建对手AI
    opponent_ = std::make_unique<Player>("AI巫师");
    opponent_->setHealth(config_.startingHealth);
    opponent_->setMaxHealth(config_.startingHealth);
    opponent_->setMana(0);
    opponent_->setMaxMana(config_.startingMana);

    // 设置初始状态
    gameState_ = GAME_INIT;
    currentPlayer_ = player_.get();
    turnNumber_ = 0;

    // 设置牌库
    setupDecks();

    // 抽初始手牌
    drawInitialHands();

    // 开始第一回合
    gameState_ = PLAYER_TURN_START;
    turnNumber_ = 1;
    startTurn(player_.get());

    addGameLog("游戏开始！", true);
    emit gameStateChanged(gameState_);
}

void GameEngine::endTurn() {
    if (!canEndTurn() || isProcessing_) {
        return;
    }

    isProcessing_ = true;

    // 结束当前回合
    endCurrentTurn();

    // 切换到对手回合
    if (currentPlayer_ == player_.get()) {
        gameState_ = OPPONENT_TURN_START;
        currentPlayer_ = opponent_.get();
        addGameLog("对手回合开始！");

        // 开始AI思考
        aiTimer_->start();
    } else {
        gameState_ = PLAYER_TURN_START;
        currentPlayer_ = player_.get();
        turnNumber_++;
        addGameLog(QString("第 %1 回合开始!").arg(turnNumber_));
    }

    emit gameStateChanged(gameState_);
    isProcessing_ = false;
}

void GameEngine::playCard(int32_t cardId, int32_t slotIndex) {
    if (currentPlayer_ != player_.get() || !canPlayCard(cardId, slotIndex)) {
        return;
    }

    std::shared_ptr<Card> card{player_->handCard(cardId)};
    if (!card) {
        return;
    }

    isProcessing_ = true;

    // 消耗法力值
    player_->setMana(player_->mana() - card->manaCost());

    // 放置到战场
    if (player_->playCardToField(cardId, slotIndex)) {
        addGameLog(QString("打出: %1").arg(card->name()));
        emit cardPlayed(card.get(), true);
        emit playerHandChanged();
        emit playerFieldChanged(slotIndex, card.get());

        // 触发进场效果
        triggerCardEffects(card.get(), "onEnter");
    }

    isProcessing_ = false;
}

void GameEngine::attack(int32_t attackerSlot, int32_t targetSlot) {
    if (currentPlayer_ != player_.get() || !canAttack(attackerSlot, targetSlot)) {
        return;
    }

    std::shared_ptr<Card> attacker{player_->fieldCardAt(attackerSlot)};
    std::shared_ptr<Card> defender{opponent_->fieldCardAt(targetSlot)};

    if (!attacker || attacker->isExhausted()) {
        return;
    }

    isProcessing_ = true;

    if (defender) {
        // 攻击生物
        resolveAttack(attacker, defender);
    } else {
        // 直接攻击玩家
        int32_t damage{attacker->attack()};
        opponent_->takeDamage(damage);

        addGameLog(QString("%1 对对手造成 %2 点伤害")
                       .arg(attacker->name())
                       .arg(damage));

        emit damageDealt(attacker, nullptr, damage);
    }

    // 标记为已攻击
    attacker->setExhausted(true);

    isProcessing_ = false;

    checkGameEnd();
}

void GameEngine::startTurn(std::unique_ptr<core::Player> &player) {
    currentPhase_ = PHASE_BEGIN;
    timeRemaining_ = config_.turnTimeLimit;

    // 重置回合计时器
    turnTimer_->start();
    emit updateTurnTimer(timeRemaining_);

    // 增加最大法力值（最多到10）
    if (player->maxMana() < config_.maxMana) {
        player->setMaxMana(player_->maxMana() + 1);
    }

    // 重置法力值
    player_->setMana(player_->maxMana()) :

        // 重置生物状态
        player_->resetFieldStatus();

    // 抽牌
    player_->drawCard();

    emit gamePhaseChanged(currentPhase_);
    addGameLog(QString("%1的回合开始").arg(player_->name()));
}

bool GameEngine::canPlayCard(int32_t cardId, int32_t slotIndex) const {
    if (currentPlayer_ != player_.get() || currentPhase_ != PHASE_MAIN) {
        return false;
    }

    std::shared_ptr<Card> card{player_->handCard(cardId)};
    if (!card) {
        return false;
    }

    // 检查发力值
    if (card->attack() > player_->mana()) {
        return false;
    }

    // 检查手牌上限
    if (player_->hand().size() > config_.maxHandSize) {
        return false;
    }

    // 检查战场空间（如果是生物）
    if (card->isCreature() && slotIndex != -1) {
        if (player_->fieldCardAt(slotIndex) != nullptr) {
            return false; // 位置已占用
        }
    }

    return true;
}

void GameEngine::processAITurn() {
    aiTimer_->stop();

    if (currentPlayer_ != opponent_.get()) {
        return;
    }

    isProcessing_ = true;

    // AI抽牌
    opponent_->drawCard();

    // AI出牌逻辑（简化版）
    int32_t availableMana{opponent_->mana()};
    auto hand(opponent_->hand());

    for (auto &card : hand) {
        if (card->manaCost() <= availableMana) {
            // 找到空位
            for (int32_t i = 0; i < 7; ++i) {
                if (!opponent_->fieldCardAt(i)) {
                    opponent_->playCardToField(card->id(), i);
                    availableMana -= card->manaCost();
                    addGameLog(QString("对手打出: %1").arg(card->name()))； break;
                }
            }
        }
    }

    // AI攻击逻辑
    for (int32_t i = 0; i < 7; ++i) {
        auto attacker(opponent_->fieldCardAt(i));
        if (attacker && !attacker->isExhausted()) {
            // 寻找攻击目标
            for (int32_t j = 0; j < 7; ++j) {
                auto defender(player_->fieldCardAt(j));
                if (defender) {
                    resolveAttack(attacker, defender);
                    break;
                }
            }
        }
    }

    // 结束AI回合
    isProcessing_ = false;
    endTurn();
}

void GameEngine::resolveAttack(const std::shared_ptr<core::Card> &attacker, const std::shared_ptr<core::Card> &defender) {
    if (!attacker || !defender) {
        return;
    }

    addGameLog(QString("%1 攻击 %2").arg(attacker->name()).arg(defender->name()));

    // 相互相造成伤害
    attacker->takeDamage(defender->attack());
    defender->takeDamage(attacker->attack());

    emit attackPerformed(attacker.get(), defender.get());

    // 检查死亡
    checkDeath();
}

void GameEngine::checkGameEnd() {
    if (player_->health() <= 0 && opponent_->health() <= 0) {
        gameState_ = GAME_END_DRAW;
        addGameLog("游戏结束：平局", true);
        emit gameEnded(false, "平局");
    } else if (player_->health() <= 0) {
        gameState_ = GAME_END_LOSE;
        addGameLog("游戏结束：你输了", true);
        emit gameEnded(false, "生命值归零");
    } else if (opponent_->health() <= 0) {
        gameState_ = GAME_END_WIN;
        addGameLog("游戏结束：你赢了", true);
        emit gameEnded(true, "对手生命值归零");
    }

    if (gameState_ >= GAME_END_WIN) {
        turnTimer_->stop();
        aiTimer_->stop();
        emit gameStateChanged(gameState_);
    }
}
} // namespace core