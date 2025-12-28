#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include "../../utils/AuthManager/AuthManager.h"
#include "../../utils/ConnectManager/ConnectManager.h"
#include "../../core/GameEngine/GameEngine.h"
#include "../widgets/CardWidget/CardWidget.h"
#include "../widgets/ClickableFrame/ClickableFrame.h"
#include "../../core/CardSystem/Card.h"
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

    // 动画
    void onCardDrawAnimation();
    void onDamageAnimation();
    void onCardDestroyAnimation();

    // 辅助函数
    void showHelpDialog();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    // 游戏控制
    void onStartGame();
    void onEndTurnClicked();
    void onDrawCardClicked();
    void onSurrenderClicked();
    void onViewGraveyardClicked();
    void onViewDeckClicked();
    void onHelpClicked();

    // 卡牌操作
    void onCardClicked(ui::CardWidget *cardWidget);
    void onCardDoubleClicked(ui::CardWidget *cardWidget);
    void onPlayerSlotClicked(int32_t slotIndex);
    void onOpponentSlotClicked(int32_t slotIndex);

    // 游戏引擎信号
    void onGameStateChanged(core::GameEngine::GameState state);
    void onGamePhaseChanged(core::GameEngine::GamePhase phase);
    void onTurnChanged(int32_t turnNumber);
    void onPlayerHealthChanged(int32_t health, int32_t maxHealth);
    void onPlayerManaChanged(int32_t mana, int32_t maxMana);
    void onOpponentHealthChanged(int32_t health, int32_t maxHealth);
    void onPlayerHandChanged();
    void onOpponentHandChanged();
    void onPlayerFieldChanged(int32_t slotIndex, core::Card *card);
    void onOpponentFieldChanged(int32_t slotIndex, core::Card *card);
    void onCardMovedToGraveyard(core::Card *card, bool isPlayerCard);
    void onDamageDealt(core::Card *source, core::Card *target, int32_t amount);
    void onCardPlayed(core::Card *card, bool isPlayerCard);
    void onAttackPerformed(core::Card *attacker, core::Card *target);
    void onGameLogAdded(const QString &log, bool isImportant);
    void onGameEnded(bool playerWon, const QString &reason);
    void onUpdateDeckInfo(int32_t playerDeckSize, int32_t opponentDeckSize);
    void onUpdateGraveyardInfo(int32_t playerGraveyardSize, int32_t opponentGraveyardSize);
    void onUpdateTurnTimer(int32_t secondsRemaining);

public:
    core::GameEngine *gameEngine_;
    // 控件引用
    QMap<int32_t, ui::ClickableFrame *> playerSlots_;
    QMap<int32_t, ui::ClickableFrame *> opponentSlots_;

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
    void updateGameLog(const QString &message, bool isImportant = false);
    void updateButtonStates();

    // 卡牌显示
    void displayPlayerHand();
    void displayPlayerField();
    void displayOpponentField();
    ui::CardWidget *createCardWidget(core::Card *card);
    void clearHandArea();
    void clearBattlefield();

    // 游戏状态
    void setGameState(core::GameEngine::GameState state);
    void showGameOverDialog(bool playerWon, const QString &reason);
    void highlightPlayableCards();
    void highlightAttackableCards();

    // 辅助函数
    void showCardInfoDialog(core::Card *card);
    void showGraveyardDialog(bool isPlayerGraveyard);
    void showDeckDialog(bool isPlayerDeck);

    // 动画
    void animateCardDraw(ui::CardWidget *cardWidget);
    void animateDamage(core::Card *target, int32_t damage);
    void animateCardAttack(core::Card *attacker, core::Card *target);
    void animateCardDestroy(core::Card *card);

    // 选择状态
    void clearSelection();
    void selectCard(ui::CardWidget *cardWidget);
    void selectSlot(int32_t slotIndex, bool isPlayerSlot);

    std::unique_ptr<Ui::MainWindow> main_w_ui_{nullptr};

    // 控件引用
    QVector<ui::CardWidget *> handCardWidgets_;
    QMap<int32_t, ui::CardWidget *> playerFieldWidgets_;
    QMap<int32_t, ui::CardWidget *> opponentFieldWidgets_;

    // 选择状态
    ui::CardWidget *selectedCard_;
    int32_t selectedSlot_;
    bool isSelectingTarget_;

    // 动画计时器
    QTimer *animationTimer_;
    QTimer *highlightTimer_;

    // 资源
    QMap<QString, QPixmap> cardImages_;
    QMap<QString, QIcon> icons_;

    // 游戏状态
    bool gameActive_;
    core::GameEngine::GameState currentState_;
};

#endif // MAINWINDOW_H
