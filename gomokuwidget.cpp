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
    qDebug() << "width:" << width;
    qDebug() << "height:" << height;
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

}

void GomokuWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resize" << event->size().height() << 'w ' << event->size().width() ;
    resize(event->size().height(), event->size().height());
    updateGeometry();
}
