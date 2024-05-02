#ifndef LABELPREVIOUSSTEP_H
#define LABELPREVIOUSSTEP_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class LabelPreviousStep : public QLabel
{
    Q_OBJECT
public:
    explicit LabelPreviousStep(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void previousStepSignal();
};

#endif // LABELPREVIOUSSTEP_H
