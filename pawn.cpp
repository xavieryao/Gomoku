#include "pawn.h"
#include <QPainter>
#include <QDebug>

Pawn::Pawn()
{
}

Pawn::State Pawn::state() const
{
    return mState;
}

void Pawn::setState(const Pawn::State &state)
{
    mState = state;
}

void Pawn::paint(QPainter *painter, QPointF& point, qreal& radius)
{
    if (mState != Pawn::NONE) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        if (mState == Pawn::WHITE) {
            painter->setBrush(Qt::white);
            painter->setPen(Qt::black);
            painter->drawEllipse(point, radius, radius);
        } else {
            painter->setBrush(Qt::black);
            painter->setPen(Qt::black);
            painter->drawEllipse(point, radius, radius);
        }
        painter->restore();
    }
}
