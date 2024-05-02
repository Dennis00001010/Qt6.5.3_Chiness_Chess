#include "gamepage.h"
#include "ui_gamepage.h"
#include <QMessageBox>

GamePage::GamePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePage)
{
    ui->setupUi(this);

    chessBoard = new QLabel(this);
    chessBoard->setPixmap(QPixmap(":/chess/chess_board.png"));

    // 定义棋盘的参数
    initXPos = 20;
    initYPos = 20;
    boardGrid = 72;

    lblWin = new QLabel(this);
    lblWin->setFixedSize(600, 72);
    lblWin->setStyleSheet("QLabel {"
                          "background-color: rgba(0, 0, 0, .7);"
                          "color: rgb(200, 200, 200);"
                          "}");
    lblWin->move(50, 345);
    QFont myPointSize;
    myPointSize.setPointSize(25);
    myPointSize.setBold(true);
    lblWin->setFont(myPointSize);
    lblWin->setAlignment(Qt::AlignCenter);

    displayTurn = new QLabel(this);
    displayTurn->setFixedSize(200, 100);
    displayTurn->move(730, 70);
    QFont myPointSize2;
    myPointSize2.setPointSize(30);
    myPointSize2.setBold(true);
    displayTurn->setFont(myPointSize2);
    displayTurn->setAlignment(Qt::AlignCenter);

    //红色框
    rbracPix = new QPixmap(":/chess/rbracket.png");
    *rbracPix = rbracPix->scaled(72, 72, Qt::KeepAspectRatio);

    rbracket = new QLabel(chessBoard);
    rbracket->setPixmap(*rbracPix);

    //黑色框
    bbracPix = new QPixmap(":/chess/bbracket.png");
    *bbracPix = bbracPix->scaled(72, 72, Qt::KeepAspectRatio);

    bbracket = new QLabel(chessBoard);
    bbracket->setPixmap(*bbracPix);

    //上一步按钮 (用 QLabel 设置图片)
    lblPreviousStep = new QLabel(this);
    lblPreviousStep->setPixmap(QPixmap(":/chess/button/btn_previous_step.png"));
    lblPreviousStep->move(750, 250);
    lblPreviousStep->setEnabled(false);
    lblPreviousStep->setCursor(Qt::PointingHandCursor);

    //把自定义的 LabelPreviousStep(QLabel类) 提升给 lblConfirm(QLabel)
    myLblPreviousStep = new LabelPreviousStep();
    previousStepLayout = new QGridLayout(this);
    previousStepLayout->addWidget(myLblPreviousStep);
    lblPreviousStep->setLayout(previousStepLayout);

    connect(myLblPreviousStep, &LabelPreviousStep::previousStepSignal, this, [=](){
        selectedPiece->x = previousX;
        selectedPiece->y = previousY;

        updateChessBoard();
        block = false;
        grabbing = false;
        lblPreviousStep->setEnabled(false);
        lblConfirm->setEnabled(false);
        selectedPiece = nullptr;
        if(eatedPiece != nullptr)
        {
            eatedPiece->pieces->setVisible(true);
        }

    });

    //确定按钮 (用 QLabel 设置图片)
    lblConfirm = new QLabel(this);
    lblConfirm->setPixmap(QPixmap(":/chess/button/btn_confirm.png"));
    lblConfirm->move(750, 350);
    lblConfirm->setEnabled(false);
    lblConfirm->setCursor(Qt::PointingHandCursor);

    //把自定义的 LabelConfirm(QLabel类) 提升给 lblConfirm(QLabel)
    myLblConfirm = new LabelConfirm();
    confirmLayout = new QGridLayout(this);
    confirmLayout->addWidget(myLblConfirm);
    lblConfirm->setLayout(confirmLayout);

    connect(myLblConfirm, &LabelConfirm::confirmSignal, this, [=](){
        bool endGame = false;

        //判断是否有棋子被吃(坐标重叠)
        for(chessPieces& piece: redTurn ? blackPieces : redPieces)
        {
            if(piece.x == selectedPiece->x && piece.y == selectedPiece->y)
            {
                eatedPiece->pieces->setVisible(true);
                eatedPiece = nullptr;
                //把被吃的棋子移到屏幕外
                piece.x = -2;
                piece.y = -2;
                updateChessBoard();

                //帅被吃游戏结束
                if(piece.name.compare(redTurn ? "bshuai" : "rshuai") == 0)
                {
                    lblWin->setText(redTurn ? "红棋赢了，游戏结束" : "黑棋赢了，游戏结束");
                    lblWin->setVisible(true);
                    endGame = true;
                }
                break;
            }
        }

        if(endGame)
        {
            block = true;
        }
        else
        {
            block = false;
            redTurn = !redTurn;
            displayTurn->setStyleSheet(redTurn ? "QLabel {"
                                                 "background-color: red;"
                                                 "color: white;"
                                                 "}" :
                                           "QLabel {"
                                           "background-color: black;"
                                           "color: white;"
                                           "}");

            displayTurn->setText(redTurn? "红棋回合" : "黑棋回合");
        }

        grabbing = false;
        lblConfirm->setEnabled(false);
        lblPreviousStep->setEnabled(false);
        selectedPiece = nullptr;
    });

    //重新开始按钮 (用 QLabel 设置图片)
    lblRestart = new QLabel(this);
    lblRestart->setPixmap(QPixmap(":/chess/button/btn_restart.png"));
    lblRestart->move(725, 550);
    lblRestart->setCursor(Qt::PointingHandCursor);

    myLblRestart = new LabelRestart();
    restartLayout = new QGridLayout(this);
    restartLayout->addWidget(myLblRestart);
    lblRestart->setLayout(restartLayout);

    connect(myLblRestart, &LabelRestart::restartSignal, this, [=](){
        QMessageBox::StandardButton answer =  QMessageBox::question(this, "重新开始", "确定要重新开始游戏吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(answer == QMessageBox::No)
        {
            return;
        }

        resetValue();

        //重置棋子坐标
        blackPieces[0].x = 0; blackPieces[0].y = 0;
        blackPieces[1].x = 1; blackPieces[1].y = 0;
        blackPieces[2].x = 2; blackPieces[2].y = 0;
        blackPieces[3].x = 3; blackPieces[3].y = 0;
        blackPieces[4].x = 4; blackPieces[4].y = 0;
        blackPieces[5].x = 5; blackPieces[5].y = 0;
        blackPieces[6].x = 6; blackPieces[6].y = 0;
        blackPieces[7].x = 7; blackPieces[7].y = 0;
        blackPieces[8].x = 8; blackPieces[8].y = 0;
        blackPieces[9].x = 1; blackPieces[9].y = 2;
        blackPieces[10].x = 7; blackPieces[10].y = 2;
        blackPieces[11].x = 0; blackPieces[11].y = 3;
        blackPieces[12].x = 2; blackPieces[12].y = 3;
        blackPieces[13].x = 4; blackPieces[13].y = 3;
        blackPieces[14].x = 6; blackPieces[14].y = 3;
        blackPieces[15].x = 8; blackPieces[15].y = 3;

        redPieces[0].x = 0; redPieces[0].y = 9;
        redPieces[1].x = 1; redPieces[1].y = 9;
        redPieces[2].x = 2; redPieces[2].y = 9;
        redPieces[3].x = 3; redPieces[3].y = 9;
        redPieces[4].x = 4; redPieces[4].y = 9;
        redPieces[5].x = 5; redPieces[5].y = 9;
        redPieces[6].x = 6; redPieces[6].y = 9;
        redPieces[7].x = 7; redPieces[7].y = 9;
        redPieces[8].x = 8; redPieces[8].y = 9;
        redPieces[9].x = 1; redPieces[9].y = 7;
        redPieces[10].x = 7; redPieces[10].y = 7;
        redPieces[11].x = 0; redPieces[11].y = 6;
        redPieces[12].x = 2; redPieces[12].y = 6;
        redPieces[13].x = 4; redPieces[13].y = 6;
        redPieces[14].x = 6; redPieces[14].y = 6;
        redPieces[15].x = 8; redPieces[15].y = 6;

        updateChessBoard();
    });

    //退出游戏按钮 (用 QLabel 设置图片)
    lblExit = new QLabel(this);
    lblExit->setPixmap(QPixmap(":/chess/button/btn_exit.png"));
    lblExit->move(725, 650);
    lblExit->setCursor(Qt::PointingHandCursor);

    myLblExit = new LabelExit();
    exitLayout = new QGridLayout(this);
    exitLayout->addWidget(myLblExit);
    lblExit->setLayout(exitLayout);

    initChessBoard();
}

GamePage::~GamePage()
{
    delete ui;
}

//初始化棋盘数据
void GamePage::initChessBoard()
{
    resetValue();

    //初始化红旗
    blackPieces[0] = {"bche", new QLabel(chessBoard), 0, 0};
    blackPieces[0].pieces->setPixmap(QPixmap(":/chess/bche.png"));
    blackPieces[1] = {"bma", new QLabel(chessBoard), 1, 0};
    blackPieces[1].pieces->setPixmap(QPixmap(":/chess/bma.png"));
    blackPieces[2] = {"bxiang", new QLabel(chessBoard), 2, 0};
    blackPieces[2].pieces->setPixmap(QPixmap(":/chess/bxiang.png"));
    blackPieces[3] = {"bshi", new QLabel(chessBoard), 3, 0};
    blackPieces[3].pieces->setPixmap(QPixmap(":/chess/bshi.png"));
    blackPieces[4] = {"bshuai", new QLabel(chessBoard), 4, 0};
    blackPieces[4].pieces->setPixmap(QPixmap(":/chess/bshuai.png"));
    blackPieces[5] = {"bshi", new QLabel(chessBoard), 5, 0};
    blackPieces[5].pieces->setPixmap(QPixmap(":/chess/bshi.png"));
    blackPieces[6] = {"bxiang", new QLabel(chessBoard), 6, 0};
    blackPieces[6].pieces->setPixmap(QPixmap(":/chess/bxiang.png"));
    blackPieces[7] = {"bma", new QLabel(chessBoard), 7, 0};
    blackPieces[7].pieces->setPixmap(QPixmap(":/chess/bma.png"));
    blackPieces[8] = {"bche", new QLabel(chessBoard), 8, 0};
    blackPieces[8].pieces->setPixmap(QPixmap(":/chess/bche.png"));

    blackPieces[9] = {"bpao", new QLabel(chessBoard), 1, 2};
    blackPieces[9].pieces->setPixmap(QPixmap(":/chess/bpao.png"));
    blackPieces[10] = {"bpao", new QLabel(chessBoard), 7, 2};
    blackPieces[10].pieces->setPixmap(QPixmap(":/chess/bpao.png"));

    blackPieces[11] = {"bbing", new QLabel(chessBoard), 0, 3};
    blackPieces[11].pieces->setPixmap(QPixmap(":/chess/bbing.png"));
    blackPieces[12] = {"bbing", new QLabel(chessBoard), 2, 3};
    blackPieces[12].pieces->setPixmap(QPixmap(":/chess/bbing.png"));
    blackPieces[13] = {"bbing", new QLabel(chessBoard), 4, 3};
    blackPieces[13].pieces->setPixmap(QPixmap(":/chess/bbing.png"));
    blackPieces[14] = {"bbing", new QLabel(chessBoard), 6, 3};
    blackPieces[14].pieces->setPixmap(QPixmap(":/chess/bbing.png"));
    blackPieces[15] = {"bbing", new QLabel(chessBoard), 8, 3};
    blackPieces[15].pieces->setPixmap(QPixmap(":/chess/bbing.png"));

    //初始化黑旗
    redPieces[0] = {"rche", new QLabel(chessBoard), 0, 9};
    redPieces[0].pieces->setPixmap(QPixmap(":/chess/rche.png"));
    redPieces[1] = {"rma", new QLabel(chessBoard), 1, 9};
    redPieces[1].pieces->setPixmap(QPixmap(":/chess/rma.png"));
    redPieces[2] = {"rxiang", new QLabel(chessBoard), 2, 9};
    redPieces[2].pieces->setPixmap(QPixmap(":/chess/rxiang.png"));
    redPieces[3] = {"rshi", new QLabel(chessBoard), 3, 9};
    redPieces[3].pieces->setPixmap(QPixmap(":/chess/rshi.png"));
    redPieces[4] = {"rshuai", new QLabel(chessBoard), 4, 9};
    redPieces[4].pieces->setPixmap(QPixmap(":/chess/rshuai.png"));
    redPieces[5] = {"rshi", new QLabel(chessBoard), 5, 9};
    redPieces[5].pieces->setPixmap(QPixmap(":/chess/rshi.png"));
    redPieces[6] = {"rxiang", new QLabel(chessBoard), 6, 9};
    redPieces[6].pieces->setPixmap(QPixmap(":/chess/rxiang.png"));
    redPieces[7] = {"rma", new QLabel(chessBoard), 7, 9};
    redPieces[7].pieces->setPixmap(QPixmap(":/chess/rma.png"));
    redPieces[8] = {"rche", new QLabel(chessBoard), 8, 9};
    redPieces[8].pieces->setPixmap(QPixmap(":/chess/rche.png"));

    redPieces[9] = {"rpao", new QLabel(chessBoard), 1, 7};
    redPieces[9].pieces->setPixmap(QPixmap(":/chess/rpao.png"));
    redPieces[10] = {"rpao", new QLabel(chessBoard), 7, 7};
    redPieces[10].pieces->setPixmap(QPixmap(":/chess/rpao.png"));

    redPieces[11] = {"rbing", new QLabel(chessBoard), 0, 6};
    redPieces[11].pieces->setPixmap(QPixmap(":/chess/rbing.png"));
    redPieces[12] = {"rbing", new QLabel(chessBoard), 2, 6};
    redPieces[12].pieces->setPixmap(QPixmap(":/chess/rbing.png"));
    redPieces[13] = {"rbing", new QLabel(chessBoard), 4, 6};
    redPieces[13].pieces->setPixmap(QPixmap(":/chess/rbing.png"));
    redPieces[14] = {"rbing", new QLabel(chessBoard), 6, 6};
    redPieces[14].pieces->setPixmap(QPixmap(":/chess/rbing.png"));
    redPieces[15] = {"rbing", new QLabel(chessBoard), 8, 6};
    redPieces[15].pieces->setPixmap(QPixmap(":/chess/rbing.png"));

    updateChessBoard();
}

void GamePage::updateChessBoard()
{
    //移动黑旗到棋盘相应的位置
    for(const chessPieces& piece: blackPieces)
    {
        piece.pieces->move(initXPos + (piece.x * boardGrid), initYPos + (piece.y * boardGrid));
    }

    //移动红旗到棋盘相应的位置
    for(const chessPieces& piece: redPieces)
    {
        piece.pieces->move(initXPos + (piece.x * boardGrid), initYPos + (piece.y * boardGrid));
    }
}

void GamePage::resetValue()
{
    //红子先下
    redTurn = true;
    //棋子尚未移动当中
    grabbing = false;

    //选中的棋子
    selectedPiece = nullptr;
    eatedPiece = nullptr;

    previousX = 0;
    previousY = 0;

    block = false;

    lblWin->setVisible(false);

    displayTurn->setStyleSheet("QLabel {"
                               "background-color: red;"
                               "color: white;"
                               "}");
    displayTurn->setText("红棋先行");

    //把红框和黑框设置为不可见
    rbracket->setVisible(false);
    bbracket->setVisible(false);
}

bool GamePage::movementRules(QString name, int i, int j)
{
    // return true;
    //---------- 行动规则的判断没有完成 -------------
    if(name.compare("bche") == 0 || name.compare("rche") == 0)
    {
        // 获取车的当前位置
        int currentX = selectedPiece->x;
        int currentY = selectedPiece->y;

        // 检查车的移动是否在同一行或同一列上
        if (currentX != i && currentY != j) {
            return false;
        }

        // 移动的路径上不能有其他棋子
        if (currentX == i) {
            // 水平方向移动
            int startY = std::min(currentY, j);
            int endY = std::max(currentY, j);
            for (int y = startY + 1; y < endY; ++y) {
                if (isPieceAt(i, y)) {
                    return false;
                }
            }
        } else if (currentY == j) {
            // 垂直方向移动
            int startX = std::min(currentX, i);
            int endX = std::max(currentX, i);
            for (int x = startX + 1; x < endX; ++x) {
                if (isPieceAt(x, j)) {
                    return false;
                }
            }
        }
        return true;
    }
    else if(name.compare("bma") == 0 || name.compare("rma") == 0)
    {
        // 获取马的当前位置
        int currentX = selectedPiece->x;
        int currentY = selectedPiece->y;

        // 定义马可以移动到的目标位置和蹩马腿的位置
        struct Move {
            int targetX, targetY;
            int legX, legY;
        };
        Move moves[] = {
            {currentX + 2, currentY + 1, currentX + 1, currentY}, // 向右移动两格，再向上移动一格
            {currentX + 2, currentY - 1, currentX + 1, currentY}, // 向右移动两格，再向下移动一格
            {currentX - 2, currentY + 1, currentX - 1, currentY}, // 向左移动两格，再向上移动一格
            {currentX - 2, currentY - 1, currentX - 1, currentY}, // 向左移动两格，再向下移动一格
            {currentX + 1, currentY + 2, currentX, currentY + 1}, // 向下移动两格，再向右移动一格
            {currentX + 1, currentY - 2, currentX, currentY - 1}, // 向下移动两格，再向左移动一格
            {currentX - 1, currentY + 2, currentX, currentY + 1}, // 向上移动两格，再向右移动一格
            {currentX - 1, currentY - 2, currentX, currentY - 1}  // 向上移动两格，再向左移动一格
        };

        // 遍历马可以移动的所有方向
        for (const auto& move : moves) {
            if (move.targetX == i && move.targetY == j) {
                // 检查蹩马腿的位置是否有棋子
                if (isPieceAt(move.legX, move.legY)) {
                    return false;
                }
                return true;
            }
        }
        return false;
    }
    else if(name.compare("bxiang") == 0 || name.compare("rxiang") == 0)
    {
        // 获取象的当前位置
        int currentX = selectedPiece->x;
        int currentY = selectedPiece->y;

        // 检查目标位置的有效性（对角线移动两个格子）
        if ((abs(currentX - i) == 2 && abs(currentY - j) == 2)) {
            // 检查象的路径上是否有障碍物
            int legX = (currentX + i) / 2;
            int legY = (currentY + j) / 2;

            if (!isPieceAt(legX, legY)) {
                // 检查象是否在自己的半场内
                if ((name == "rxiang" && j >= 5) || (name == "bxiang" && j <= 4)) {
                    return true;
                }
            }
        }
        return false;
    }
    else if(name.compare("bshi") == 0 || name.compare("rshi") == 0)
    {
        // 获取仕的当前位置
        int currentX = selectedPiece->x;
        int currentY = selectedPiece->y;

        // 检查仕的目标位置是否符合斜向移动一步的规则
        if ((abs(currentX - i) == 1 && abs(currentY - j) == 1)) {
            // 检查目标位置是否在自己的九宫内
            if (name == "rshi") {
                // 红仕只能在红方的九宫格内
                if (i >= 3 && i <= 5 && j >= 7 && j <= 9) {
                    return true;
                }
            } else if (name == "bshi") {
                // 黑仕只能在黑方的九宫格内
                if (i >= 3 && i <= 5 && j >= 0 && j <= 2) {
                    return true;
                }
            }
        }
        return false;
    }
    else if(name.compare("bshuai") == 0 || name.compare("rshuai") == 0)
    {
        // 获取帅或将的当前位置
        int currentX = selectedPiece->x;
        int currentY = selectedPiece->y;

        // 检查帅或将的目标位置是否是直线移动一步
        if ((abs(currentX - i) == 1 && currentY == j) || (abs(currentY - j) == 1 && currentX == i)) {
            // 检查目标位置是否在自己的九宫内
            if (name == "rshuai") {
                // 红帅只能在红方的九宫格内
                if (i >= 3 && i <= 5 && j >= 7 && j <= 9) {
                    return true;
                }
            } else if (name == "bshuai") {
                // 黑将只能在黑方的九宫格内
                if (i >= 3 && i <= 5 && j >= 0 && j <= 2) {
                    return true;
                }
            }
        }
        // 如果移动不符合直线移动一步或者不在自己的九宫内，则返回 false
        return false;
    }
    else if(name.compare("bpao") == 0 || name.compare("rpao") == 0)
    {
        int currentX = selectedPiece->x;
        int currentY = selectedPiece->y;

        // 检查目标位置是否是炮在横向或纵向的移动
        if (currentX != i && currentY != j) {
            // 如果既不在同一行也不在同一列，则移动不合法
            return false;
        }

        // 判断方向
        bool isHorizontal = (currentY == j);
        bool isVertical = (currentX == i);

        // 用于记录路径中是否跳过一个棋子
        bool hasJumped = false;

        // 判断移动方向和路径
        if (isHorizontal) {
            // 水平方向移动
            int step = (i > currentX) ? 1 : -1;
            for (int x = currentX + step; x != i; x += step) {
                if (isPieceAt(x, j)) {
                    // 如果途中遇到一个棋子
                    if (hasJumped) {
                        // 如果已经跳过一个棋子，再遇到一个棋子，则返回 false
                        return false;
                    }
                    // 标记已经跳过一个棋子
                    hasJumped = true;
                }
            }
        } else if (isVertical) {
            // 垂直方向移动
            int step = (j > currentY) ? 1 : -1;
            for (int y = currentY + step; y != j; y += step) {
                if (isPieceAt(i, y)) {
                    // 如果途中遇到一个棋子
                    if (hasJumped) {
                        // 如果已经跳过一个棋子，再遇到一个棋子，则返回 false
                        return false;
                    }
                    // 标记已经跳过一个棋子
                    hasJumped = true;
                }
            }
        }

        // 如果到达目标位置时，路径上只跳过一个棋子或没有跳过棋子
        if (isPieceAt(i, j)) {
            // 如果目标位置上有棋子，则路径上必须跳过一个棋子
            return hasJumped;
        } else {
            // 如果目标位置上没有棋子，则不能跳过任何棋子
            return !hasJumped;
        }
    }
    //黑兵
    else if(name.compare("bbing") == 0)
    {
        // 判断黑兵的位置（是否过河）
        if(selectedPiece->y <= 4)
        {
            // 黑兵未过河时只能向前移动一步
            if(selectedPiece->x == i && selectedPiece->y + 1 == j)
            {
                return true;
            }
        }
        // 黑兵过河后可以向前或左右移动一步
        else if(selectedPiece->y >=5)
        {
            if(((selectedPiece->x + 1 == i || selectedPiece->x - 1 == i) && selectedPiece->y == j) || (selectedPiece->y + 1 == j && selectedPiece->x == i))
            {
                return true;
            }
        }
    }
    //红兵
    else if(name.compare("rbing") == 0)
    {
        // 判断红兵的位置（是否过河）
        if(selectedPiece->y >= 5)
        {
            // 红兵未过河时只能向前移动一步
            if(selectedPiece->x == i && selectedPiece->y - 1 == j)
            {
                return true;
            }
        }
        // 红兵过河后可以向前或左右移动一步
        else if(selectedPiece->y <= 4)
        {
            if(((selectedPiece->x - 1 == i || selectedPiece->x + 1 == i) && selectedPiece->y == j) || (selectedPiece->y - 1 == j && selectedPiece->x == i))
            {
                return true;
            }
        }
    }
    return false;
}

bool GamePage::isPieceAt(int x, int y)
{
    for (const chessPieces& piece : redPieces) {
        if (piece.x == x && piece.y == y) {
            return true;
        }
    }
    for (const chessPieces& piece : blackPieces) {
        if (piece.x == x && piece.y == y) {
            return true;
        }
    }
    return false;
}

LabelExit* GamePage::getLblExit()
{
    return myLblExit;
}

void GamePage::mousePressEvent(QMouseEvent *event)
{
    if(block)
    {
        return;
    }

    //获取点击位置
    int clickX = event->pos().x();
    int clickY = event->pos().y();

    if(grabbing)
    {
        //格子的中心
        int centerX = 0;
        int centerY = 0;

        bool withinXRange = false;
        bool withinYRange = false;

        //棋盘列
        for(int i=0; i<9; i++)
        {
            //棋盘行
            for(int j=0; j<10; j++)
            {
                centerX = initXPos + 30 + (i * boardGrid);
                centerY = initYPos + 30 + (j * boardGrid);

                withinXRange = clickX >= (centerX - 25) && clickX <= (centerX + 25);
                withinYRange = clickY >= (centerY - 25) && clickY <= (centerY + 25);

                if(withinXRange && withinYRange)
                {
                    for(chessPieces& piece: redTurn ? redPieces : blackPieces)
                    {
                        if(piece.x == i && piece.y == j)
                        {
                            selectedPiece = &piece;
                            (redTurn ? rbracket : bbracket)->move(centerX-30, centerY-30);
                            return;
                        }
                    }

                    bool check = movementRules(selectedPiece->name, i, j);
                    if(!check)
                    {
                        return;
                    }

                    (redTurn ? rbracket : bbracket)->move(centerX-30, centerY-30);

                    //保存现在棋子的位置
                    previousX = selectedPiece->x;
                    previousY = selectedPiece->y;

                    //更新现在棋子的位置
                    selectedPiece->x = i;
                    selectedPiece->y = j;

                    block = true;
                    lblConfirm->setEnabled(true);
                    lblPreviousStep->setEnabled(true);

                    for(chessPieces& piece: redTurn ? blackPieces : redPieces)
                    {
                        if(piece.x == selectedPiece->x && piece.y == selectedPiece->y)
                        {
                            eatedPiece = &piece;
                            eatedPiece->pieces->setVisible(false);
                            break;
                        }
                    }

                    updateChessBoard();
                    return;
                }
            }
        }
    }

    int index = 0;
    for(const chessPieces& piece: redTurn ? redPieces : blackPieces)
    {
        //计算棋子的中心位置
        int centerX = initXPos + (piece.x * boardGrid) + 30;
        int centerY = initYPos + (piece.y * boardGrid) + 30;

        // 判断点击是否在棋子中心的 25 像素范围内
        bool withinXRange = clickX >= (centerX - 25) && clickX <= (centerX + 25);
        bool withinYRange = clickY >= (centerY - 25) && clickY <= (centerY + 25);

        if(withinXRange && withinYRange)
        {
            grabbing = true;
            selectedPiece = (redTurn ? redPieces : blackPieces) + index;

            //显示红色框 并 移动到相应的棋子上
            (redTurn ? rbracket : bbracket)->move(centerX-28, centerY-28);
            (redTurn ? rbracket : bbracket)->setVisible(true);
            break;
        }
        index++;
    }
}
