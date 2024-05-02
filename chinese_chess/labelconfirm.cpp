#include "labelconfirm.h"

LabelConfirm::LabelConfirm(QWidget *parent)
    : QLabel{parent}
{}

void LabelConfirm::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit confirmSignal();
    }
}
