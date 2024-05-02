#include "titlepage.h"
#include <QPainter>

TitlePage::TitlePage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(1000, 829);

    titleImage = new QPixmap(":/chess/title.png");
    lblImage = new QPixmap(":/chess/button/btn_start.png");

    myLblStart = new LabelStart();

    layout = new QGridLayout(this);
    layout->addWidget(myLblStart);
    layout->setContentsMargins(0, 0, 0, 0);

    lblStart = new QLabel(this);
    lblStart->setLayout(layout);
    lblStart->setPixmap(*lblImage);
    lblStart->move(width() / 2 - lblImage->width() / 2, height() / 2 + 200);
    lblStart->setCursor(Qt::PointingHandCursor);
}

LabelStart* TitlePage::getMyLblStart()
{
    return myLblStart;
}

QLabel* TitlePage::getLblStart()
{
    return lblStart;
}

void TitlePage::paintEvent(QPaintEvent *event)
{
    QPainter painter = QPainter(this);
    painter.drawPixmap(width() / 2 - titleImage->width() / 2, height() / 2 - titleImage->height(), titleImage->width(), titleImage->height(), *titleImage);
}
