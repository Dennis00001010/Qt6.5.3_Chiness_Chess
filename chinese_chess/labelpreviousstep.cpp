#include "labelpreviousstep.h"

LabelPreviousStep::LabelPreviousStep(QWidget *parent)
    : QLabel{parent}
{}

void LabelPreviousStep::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit previousStepSignal();
    }
}
