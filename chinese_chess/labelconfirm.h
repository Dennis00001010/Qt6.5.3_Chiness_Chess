#ifndef LABELCONFIRM_H
#define LABELCONFIRM_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class LabelConfirm : public QLabel
{
    Q_OBJECT
public:
    explicit LabelConfirm(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void confirmSignal();
};

#endif // LABELCONFIRM_H
