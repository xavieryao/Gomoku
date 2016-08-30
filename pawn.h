#ifndef PAWN_H
#define PAWN_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPixmap>
#include <QGraphicsView>
#include "gomokuwidget.h"

class Pawn : public QGraphicsItem
{
public:
    Pawn(qreal width, GomokuWidget* view);

    enum Color {
        BLACK, WHITE
    };

    Color color() const;
    void setColor(const Color &color);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    Color mColor;
    qreal width;
    QPixmap mBlackPix, mWhitePix;
    GomokuWidget* mView;
};

#endif // PAWN_H
