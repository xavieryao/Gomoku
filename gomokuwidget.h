#ifndef GOMOKUWIDGET_H
#define GOMOKUWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QRectF>
#include <QResizeEvent>
#include <QPoint>

class GomokuWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GomokuWidget(QWidget *parent = 0);
    ~GomokuWidget();

    void drawBackground(QPainter* painter, const QRectF &rect);
    void resizeEvent(QResizeEvent *event);

    bool hasHeightForWidth() const {return true;}
    int heightForWidth(int w) const {return w;}

private:
    QGraphicsScene* mScene;

    QPointF pointForPos(int x, int y, const QRectF &rect);
};

#endif // GOMOKUWIDGET_H
