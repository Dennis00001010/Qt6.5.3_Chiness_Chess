#ifndef TITLEPAGE_H
#define TITLEPAGE_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include "labelstart.h"

class TitlePage : public QWidget
{
    Q_OBJECT

public:
    explicit TitlePage(QWidget *parent = nullptr);

    LabelStart* getMyLblStart();
    QLabel* getLblStart();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap *titleImage;
    QPixmap *lblImage;
    QLabel *lblStart;
    QGridLayout *layout;
    LabelStart *myLblStart;
};

#endif // TITLEPAGE_H
