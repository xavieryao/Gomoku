#include "pawn.h"
#include <QPainter>
#include <QDebug>

Pawn::Pawn(qreal width, GomokuWidget* view) :
    QGraphicsItem(0),
    mView(view),
    width(width)
{
    mBlackPix = QPixmap(width, width);
    mBlackPix.load(":/black");

    mWhitePix = QPixmap(width, width);
    mWhitePix.load(":/white");
}

Pawn::Color Pawn::color() const
{
    return mColor;
}

void Pawn::setColor(const Pawn::Color &color)
{
    mColor = color;
}

QRectF Pawn::boundingRect() const
{
    qDebug() << "bounding rect";

    return QRectF(-width/2, -width/2,
                  width/2, width/2);
}

void Pawn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << "paint pawn";
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);

//    painter->drawPixmap(pos(), mBlackPix);
    qDebug() << pos();
    painter->drawEllipse(mView->pointForPos(10, 10), 10, 10);
}
