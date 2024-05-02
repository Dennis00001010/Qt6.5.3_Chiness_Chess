#include "labelexit.h"

LabelExit::LabelExit(QWidget *parent)
    : QLabel{parent}
{}

void LabelExit::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit exitSignal();
    }
}
