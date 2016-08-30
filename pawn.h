#ifndef PAWN_H
#define PAWN_H

#include <QRectF>
#include <QPixmap>
#include <QPainter>

class Pawn
{
public:
    Pawn();

    enum State {
        NONE, BLACK, WHITE
    };

    State state() const;
    void setState(const State &color);
    void paint(QPainter* painter, QPointF& point, qreal& radius);

    int x,y;
private:
    State mState = NONE;
};

#endif // PAWN_H
