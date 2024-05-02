#ifndef LABELSTART_H
#define LABELSTART_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class LabelStart : public QLabel
{
    Q_OBJECT
public:
    explicit LabelStart(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void startSignal();
};

#endif // LABELSTART_H
