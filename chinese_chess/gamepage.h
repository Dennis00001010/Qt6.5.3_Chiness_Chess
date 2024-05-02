#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>
#include <QGridLayout>
#include "labelconfirm.h"
#include "labelpreviousstep.h"
#include "labelrestart.h"
#include "labelexit.h"

namespace Ui {
class GamePage;
}

//定义结构体保存棋子的信息
struct chessPieces
{
    QString name; //保存棋子的信息
    QLabel *pieces; //使用QLabel设置棋子的图片
    int x; //保存棋子在棋盘x的偏移量
    int y; //保存棋子在棋盘y的偏移量

    //注意: 棋盘左上角的坐标 （黑车的位置） 视为(0，0)
};

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();

    void initChessBoard(); //初始化棋盘
    void updateChessBoard(); //更新棋子的位置
    void resetValue(); //重置棋盘数据
    bool movementRules(QString name, int i, int j); //判断棋子的移动规则
    bool isPieceAt(int x, int y); //判断在棋盘上某个位置是否有棋子
    LabelExit* getLblExit();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::GamePage *ui;

    QLabel *chessBoard;
    int initXPos;
    int initYPos;
    int boardGrid;
    struct chessPieces redPieces[16];
    struct chessPieces blackPieces[16];
    struct chessPieces *selectedPiece; //选中的棋子
    struct chessPieces *eatedPiece; //被吃的棋子

    bool redTurn; //是否轮到红子下棋 默认true，否则则轮到黑子下棋
    bool grabbing; //棋子是否在移动当中， 默认false

    QPixmap *rbracPix;
    QLabel *rbracket;
    QPixmap *bbracPix;
    QLabel *bbracket;

    QLabel *lblConfirm;
    QGridLayout *confirmLayout;
    LabelConfirm *myLblConfirm;

    QLabel *lblPreviousStep;
    QGridLayout *previousStepLayout;
    LabelPreviousStep *myLblPreviousStep;
    int previousX;
    int previousY;
    bool block;

    QLabel *lblWin;
    QLabel *displayTurn;

    QLabel *lblRestart;
    QGridLayout *restartLayout;
    LabelRestart *myLblRestart;

    QLabel *lblExit;
    QGridLayout *exitLayout;
    LabelExit *myLblExit;
};

#endif // GAMEPAGE_H
