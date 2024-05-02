#ifndef LABELRESTART_H
#define LABELRESTART_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class LabelRestart : public QLabel
{
    Q_OBJECT
public:
    explicit LabelRestart(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void restartSignal();
};

#endif // LABELRESTART_H
