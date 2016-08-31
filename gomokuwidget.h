#ifndef GOMOKUWIDGET_H
#define GOMOKUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QResizeEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include "map.h"

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

    void setColor(Pawn::State color) {current = color;}

    QPoint pointForPosition();

signals:
    void move(const QPoint & position);
    void win(Pawn::State color);

private:
    Map mMap;
    qreal mPawnWidth = 0;

    Pawn::State current = Pawn::BLACK;

    bool hasWon();
    bool test(QPoint position);
    Pawn::State flip(Pawn::State state);
    QList<QPoint> hint();
    int goodDirections(QPoint position);

    const int HORIZONTAL = 1;
    const int VERTICAL = HORIZONTAL << 1;
    const int CROSS = VERTICAL << 1;
    const int BACK_CROSS = CROSS << 1;

    QList<QPoint> hintList;

public slots:
    void positionPawn(QPoint position);

};

#endif // GOMOKUWIDGET_H
