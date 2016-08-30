#ifndef GOMOKUWIDGET_H
#define GOMOKUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QResizeEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QMouseEvent>

class GomokuWidget : public QWidget
{
    Q_OBJECT

public:
    GomokuWidget(QWidget *parent = 0);
    ~GomokuWidget();

    void paintEvent(QPaintEvent* event);
    void drawBackground(QPainter* painter, const QRectF &rect);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool hasHeightForWidth() const {return true;}
    int heightForWidth(int w) const {return w;}

    QPointF pointForPos(int x, int y);

private:
    qreal pawnWidth;

};

#endif // GOMOKUWIDGET_H
