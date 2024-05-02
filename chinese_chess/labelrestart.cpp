#include "labelrestart.h"

LabelRestart::LabelRestart(QWidget *parent)
    : QLabel{parent}
{}

void LabelRestart::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit restartSignal();
    }
}
