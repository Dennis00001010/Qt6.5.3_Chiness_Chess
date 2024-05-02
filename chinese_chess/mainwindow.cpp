#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("中国双人象棋");
    setFixedSize(1000, 829);

    // 棋盘图的大小 (697, 786);

    titlePage = new TitlePage();
    gamePage = new GamePage();

    setCentralWidget(titlePage);

    QPushButton *btnAbout = new QPushButton("关于Qt", this);
    btnAbout->setFixedSize(100, 40);
    btnAbout->move(800, 700);
    btnAbout->setCursor(Qt::PointingHandCursor);
    btnAbout->setStyleSheet("QPushButton {"
                            "background-color: rgba(0, 0, 0, .4);"
                            "color: rgb(200, 200, 200);"
                            "border: 1px solid red ;"
                            "}");

    connect(btnAbout, &QPushButton::clicked, this, [=](){
        QMessageBox::aboutQt(this);
    });

    connect(titlePage->getMyLblStart(), &LabelStart::startSignal, this, [=](){
        titlePage->getLblStart()->setCursor(Qt::ArrowCursor);
        setCentralWidget(gamePage);
        btnAbout->setVisible(false);
    });

    connect(gamePage->getLblExit(), &LabelExit::exitSignal, this, [=](){
        QMessageBox::StandardButton answer =  QMessageBox::question(this, "退出", "确定要退出游戏吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(answer == QMessageBox::No)
        {
            return;
        }
        close();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter = QPainter(this);

    QPixmap pix = QPixmap(":/chess/background/bg2.jpg");
    pix = pix.scaled(width(), height(), Qt::KeepAspectRatio);

    painter.drawPixmap(0, 0, width(), height(), pix);
}
