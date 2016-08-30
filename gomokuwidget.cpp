#include "gomokuwidget.h"
#include <QPixmap>
#include <QDebug>
#include <QSizePolicy>

GomokuWidget::GomokuWidget(QWidget *parent)
    : QGraphicsView(parent),
    mScene(new QGraphicsScene(this))
{

    mScene->setSceneRect(0, 0, 300, 300);
    qDebug("set scene");
    setScene(mScene);
    qDebug("add item");
}

GomokuWidget::~GomokuWidget()
{

}

void GomokuWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    // paint wooden texture
    painter->setPen(Qt::NoPen);
    QPixmap bg(":/img/wood_texture");
    painter->setBrush(bg);
    painter->drawRect(rect);

    // paint board
    qreal startX = rect.topLeft().x() + rect.width()*0.05;
    qreal startY = rect.topLeft().y() + rect.height()*0.05;
    qreal width = rect.width()*0.9;
    qreal height = rect.height()*0.9;
    qreal blockWidth = width / 14;
    qreal blockHeight = height / 14;

    QPen pen = QPen(Qt::black);
    pen.setWidthF(blockWidth*0.05);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(pen);

    // draw h lines
    qreal y = startY;
    for (int i = 0; i < 15; i++) {
        painter->drawLine(startX, y, startX + width, y);
        y += blockHeight;
    }

    // draw v lines
    qreal x = startX;
    for (int i = 0; i < 15; i++) {
        painter->drawLine(x, startY, x, startY + height);
        x += blockWidth;
    }

    // draw point
    qreal radius = blockWidth*0.13;
    painter->setBrush(Qt::black);

    for (int i = 3; i <= 12; i+= 4) {
        for (int j = 3; j <=12; j+=4) {
            QPointF point = pointForPos(i, j, rect);
            painter->drawEllipse(point, radius, radius);
        }
    }
}

void GomokuWidget::resizeEvent(QResizeEvent *event)
{
    resize(event->size().height(), event->size().height());
    updateGeometry();
}

QPointF GomokuWidget::pointForPos(int x, int y, const QRectF &rect)
{

    qreal startX = rect.topLeft().x() + rect.width()*0.05;
    qreal startY = rect.topLeft().y() + rect.height()*0.05;
    qreal width = rect.width()*0.9/14;
    qreal height = rect.height()*0.9/14;
    return QPoint(startX + width*x, startY + height*y);
}
