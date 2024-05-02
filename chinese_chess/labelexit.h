#ifndef LABELEXIT_H
#define LABELEXIT_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class LabelExit : public QLabel
{
    Q_OBJECT
public:
    explicit LabelExit(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void exitSignal();
};

#endif // LABELEXIT_H
