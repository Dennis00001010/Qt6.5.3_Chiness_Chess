#include "labelstart.h"

LabelStart::LabelStart(QWidget *parent)
    : QLabel{parent}
{}

void LabelStart::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit startSignal();
    }
}
