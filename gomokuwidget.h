#ifndef GOMOKUWIDGET_H
#define GOMOKUWIDGET_H

#include <QWidget>

class GomokuWidget : public QWidget
{
    Q_OBJECT

public:
    GomokuWidget(QWidget *parent = 0);
    ~GomokuWidget();
};

#endif // GOMOKUWIDGET_H
