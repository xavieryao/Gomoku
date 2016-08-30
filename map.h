#ifndef TYPES_H
#define TYPES_H

#include <QMap>
#include <QPointF>
#include <QPoint>
#include "pawn.h"
#include <QDebug>


class Map : public QVector<QVector<Pawn> >{
public:
    Map(){
        qDebug() << "map";

        qDebug() << "size" << this->size();
    }

    static QPointF positionForPoint(QPoint pos, const QRectF & rect)
    {
        qreal startX = rect.topLeft().x() + rect.width()*0.05;
        qreal startY = rect.topLeft().y() + rect.height()*0.05;
        qreal width = rect.width()*0.9/14;
        qreal height = rect.height()*0.9/14;
        return QPointF(startX + width*pos.x(), startY + height*pos.y());
    }

    static QPoint pointForPosition(QPointF pos, const QRectF & rect)
    {
        qreal startX = rect.topLeft().x() + rect.width()*0.05;
        qreal startY = rect.topLeft().y() + rect.height()*0.05;
        qreal width = rect.width()*0.9/14;
        qreal height = rect.height()*0.9/14;
        qreal x = (pos.x()-startX)/width;
        qreal y = (pos.y()-startY)/height;
        int intX, intY;
        if (x - ((int)x) > 0.5) {
            intX = x;
            intX++;
        } else {
            intX = x;
        }
        if (y - ((int)y) > 0.5) {
            intY = y;
            intY++;
        } else {
            intY = y;
        }
        if (intX >= 15 || intY >= 15) {
            return QPoint(-1, -1);
        }
        return QPoint(intX, intY);
    }
};

#endif // TYPES_H
