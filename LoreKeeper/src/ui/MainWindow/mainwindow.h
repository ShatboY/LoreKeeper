#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include "../../utils/AuthManager/AuthManager.h"
#include "../../utils/ConnectManager/ConnectManager.h"
#include "../../core/GameEngine/GameEngine.h"
#include <QMessageBox>
#include <QStatusBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 游戏控制
    void onStartGame();
    void onEndTurnClicked();
    void onDrawCardClicked();
    void onSurrenderClicked();
    void onViewGraveyardClicked();
    void onViewDeckClicked();
    void onHelpClicked();

    // 卡牌操作
    void onCardClicked(CardWidget* cardWidget);
    void onCardDoubleClicked(CardWidget* cardWidget);
    void onPlayerSlotClicked(int slotIndex);
    void onOpponentSlotClicked(int slotIndex);

    // 游戏引擎信号
    void onGameStateChanged(GameEngine::GameState state);
    void onGamePhaseChanged(GameEngine::GamePhase phase);
    void onTurnChanged(int turnNumber);
    void onPlayerHealthChanged(int health, int maxHealth);
    void onPlayerManaChanged(int mana, int maxMana);
    void onOpponentHealthChanged(int health, int maxHealth);
    void onPlayerHandChanged();
    void onOpponentHandChanged();
    void onPlayerFieldChanged(int slotIndex, Card* card);
    void onOpponentFieldChanged(int slotIndex, Card* card);
    void onCardMovedToGraveyard(Card* card, bool isPlayerCard);
    void onDamageDealt(Card* source, Card* target, int amount);
    void onCardPlayed(Card* card, bool isPlayerCard);
    void onAttackPerformed(Card* attacker, Card* target);
    void onGameLogAdded(const QString& log, bool isImportant);
    void onGameEnded(bool playerWon, const QString& reason);
    void onUpdateDeckInfo(int playerDeckSize, int opponentDeckSize);
    void onUpdateGraveyardInfo(int playerGraveyardSize, int opponentGraveyardSize);
    void onUpdateTurnTimer(int secondsRemaining);

    // 动画
    void onCardDrawAnimation();
    void onDamageAnimation();
    void onCardDestroyAnimation();

private:
    // 初始化
    void initializeUI();
    void setupConnections();
    void loadResources();
    void applyStyleSheet();

    // UI更新
    void updatePlayerInfo();
    void updateOpponentInfo();
    void updateTurnInfo();
    void updateBattlefield();
    void updateHandArea();
    void updateGameLog(const QString& message, bool isImportant = false);
    void updateButtonStates();

    // 卡牌显示
    void displayPlayerHand();
    void displayPlayerField();
    void displayOpponentField();
    CardWidget* createCardWidget(Card* card);
    void clearHandArea();
    void clearBattlefield();

    // 游戏状态
    void setGameState(GameEngine::GameState state);
    void showGameOverDialog(bool playerWon, const QString& reason);
    void highlightPlayableCards();
    void highlightAttackableCards();

    // 辅助函数
    void showCardInfoDialog(Card* card);
    void showGraveyardDialog(bool isPlayerGraveyard);
    void showDeckDialog(bool isPlayerDeck);
    void showHelpDialog();

    // 动画
    void animateCardDraw(CardWidget* cardWidget);
    void animateDamage(Card* target, int damage);
    void animateCardAttack(Card* attacker, Card* target);
    void animateCardDestroy(Card* card);

    // 选择状态
    void clearSelection();
    void selectCard(CardWidget* cardWidget);
    void selectSlot(int slotIndex, bool isPlayerSlot);

    std::unique_ptr<Ui::MainWindow> main_w_ui_{nullptr};
    std::unique_ptr<GameEngine> gameEngine_;

    // 控件引用
    QMap<int, ClickableFrame*> m_playerSlots;
    QMap<int, ClickableFrame*> m_opponentSlots;
    QVector<CardWidget*> m_handCardWidgets;
    QMap<int, CardWidget*> m_playerFieldWidgets;
    QMap<int, CardWidget*> m_opponentFieldWidgets;

    // 选择状态
    CardWidget* m_selectedCard;
    int m_selectedSlot;
    bool m_isSelectingTarget;

    // 动画计时器
    QTimer* m_animationTimer;
    QTimer* m_highlightTimer;

    // 资源
    QMap<QString, QPixmap> m_cardImages;
    QMap<QString, QIcon> m_icons;

    // 游戏状态
    bool m_gameActive;
    GameEngine::GameState m_currentState;
};

#endif // MAINWINDOW_H
