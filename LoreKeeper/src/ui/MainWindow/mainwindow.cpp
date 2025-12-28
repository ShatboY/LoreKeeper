#include "mainwindow.h"
#include "../../utils/DatabaseManager/BaseManager/DatabaseManager.h"
#include "../../utils/LoggerManager/Logger.h"

#include <QFile>
#include <QStyle>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    main_w_ui_(std::make_unique<Ui::MainWindow>()),
    gameEngine_(&core::GameEngineGetInstance()),
    selectedCard_(nullptr),
    selectedSlot_(-1),
    isSelectingTarget_(false),
    gameActive_(false),
    currentState_(core::GameEngine::GAME_INIT) {
    main_w_ui_->setupUi(this);

    // 初始化
    initializeUI();
    utils::ConnectManager::SetupConnections(this, main_w_ui_.get());
    loadResources();
    applyStyleSheet();

    // 创建动画计时器
    animationTimer_ = new QTimer(this);
    animationTimer_->setInterval(16); // -60 FPS

    highlightTimer_ = new QTimer(this);
    highlightTimer_->setInterval(500); // 高亮闪烁

    // TODO 添加日志
}

void MainWindow::initializeUI() {
    // 设置窗口属性
    setWindowTitle("LoreKeeper - game");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    //    setFixedSize(1280, 720);

    // 初始化进度条
    main_w_ui_->playerHealthBar->setRange(0, 20);
    main_w_ui_->playerHealthBar->setValue(20);
    main_w_ui_->playerHealthBar->setFormat("%v/%m");
    main_w_ui_->playerHealthBar->setAlignment(Qt::AlignCenter);

    main_w_ui_->playerManaBar->setRange(0, 10);
    main_w_ui_->playerManaBar->setValue(0);
    main_w_ui_->playerManaBar->setFormat("%v/%m");
    main_w_ui_->playerManaBar->setAlignment(Qt::AlignCenter);

    main_w_ui_->turnTimerBar->setRange(0, 60);
    main_w_ui_->turnTimerBar->setValue(60);
    main_w_ui_->turnTimerBar->setTextVisible(false);

    // 设置日志文本框
    main_w_ui_->gameLogTextEdit->setReadOnly(true);
    main_w_ui_->gameLogTextEdit->setFont(QFont("Consolas", 9));
    main_w_ui_->gameLogTextEdit->setStyleSheet(
        "QTextEdit { background-color: #1a1a1a; color: #e0e0e0; border: 1px solid #333; }");

    // 设置手牌滚动区域
    main_w_ui_->handCardsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    main_w_ui_->handCardsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    main_w_ui_->handCardsScrollArea->setWidgetResizable(true);

    // 创建手牌容器
    QWidget *handContainer{new QWidget()};
    QHBoxLayout *handLayout{new QHBoxLayout(handContainer)};
    handLayout->setSpacing(10);
    handLayout->setContentsMargins(10, 10, 10, 10);
    handLayout->setAlignment(Qt::AlignLeft);
    main_w_ui_->handCardsScrollArea->setWidget(handContainer);

    // 收集战场槽位
    for (int32_t i = 1; i <= 7; ++i) {
        ui::ClickableFrame *playerSlot{
            findChild<ui::ClickableFrame *>(QString("playerSlot%1").arg(i))};
        ui::ClickableFrame *opponentSlot{
            findChild<ui::ClickableFrame *>(QString("opponentSlot%1").arg(i))};

        if (playerSlot) {
            playerSlots_[i - 1] = playerSlot;
            playerSlot->setObjectName(QString("playerSlot%1").arg(i));
            playerSlot->setStyleSheet(
                "ClickableFrame { border: 2px dashed #666; background-color: rgba(100, 100, 100, 50); }"
                "ClickableFrame:hover { border: 2px solid #4CAF50; background-color: rgba(76, 175, 80, 30); }"
            );
        }

        if (opponentSlot) {
            opponentSlots_[i - 1] = opponentSlot;
            opponentSlot->setObjectName(QString("opponentSlot%1").arg(i));
            opponentSlot->setStyleSheet(
                "ClickableFrame { border: 2px dashed #666; background-color: rgba(100, 100, 100, 50); }"
                "ClickableFrame:hover { border: 2px solid #FF5722; background-color: rgba(255, 87, 34, 30); }"
            );
        }
    }

    // 设置按钮样式
    main_w_ui_->endTurnButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:disabled { background-color: #cccccc; }"
    );

    main_w_ui_->surrenderButton->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; border-radius: 5px; }"
        "QPushButton:hover { background-color: #d32f2f; }"
    );

    // 禁用初始按钮状态
    updateButtonStates();

    // 显示欢迎消息
    updateGameLog("欢迎来到LoreKeeper！点击右上角菜单开始游戏。", true);
}


