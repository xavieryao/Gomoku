#include "gomokuwidget.h"
#include <QPixmap>
#include <QDebug>
#include <QSizePolicy>
#include "pawn.h"

GomokuWidget::GomokuWidget(QWidget *parent)
    : QWidget(parent)
{

}

GomokuWidget::~GomokuWidget()
{

}

void GomokuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRectF wholeRect = this->rect();
    qDebug() << "drawBackground";
    // paint wooden texture
    painter.setPen(Qt::NoPen);
    QPixmap bg(":/img/wood_texture");
    painter.setBrush(bg);
    painter.drawRect(wholeRect);

    // paint board
    qreal startX = wholeRect.topLeft().x() + wholeRect.width()*0.05;
    qreal startY = wholeRect.topLeft().y() + wholeRect.height()*0.05;
    qreal width = wholeRect.width()*0.9;
    qreal height = wholeRect.height()*0.9;
    qreal blockWidth = width / 14;
    qreal blockHeight = height / 14;

    QPen pen = QPen(Qt::black);
    pen.setWidthF(blockWidth*0.05);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);

    // draw h lines
    qreal y = startY;
    for (int i = 0; i < 15; i++) {
        painter.drawLine(startX, y, startX + width, y);
        y += blockHeight;
    }

    // draw v lines
    qreal x = startX;
    for (int i = 0; i < 15; i++) {
        painter.drawLine(x, startY, x, startY + height);
        x += blockWidth;
    }

    // draw point
    qreal radius = blockWidth*0.13;
    painter.setBrush(Qt::black);

    for (int i = 3; i <= 12; i+= 4) {
        for (int j = 3; j <=12; j+=4) {
            QPointF point = pointForPos(i, j);
            painter.drawEllipse(point, radius, radius);
        }
    }

    pawnWidth = blockWidth*0.4;
}

void GomokuWidget::resizeEvent(QResizeEvent *event)
{
    resize(event->size().height(), event->size().height());
    updateGeometry();
}

QPointF GomokuWidget::pointForPos(int x, int y)
{

    qreal startX = rect().topLeft().x() + rect().width()*0.05;
    qreal startY = rect().topLeft().y() + rect().height()*0.05;
    qreal width = rect().width()*0.9/14;
    qreal height = rect().height()*0.9/14;
    return QPoint(startX + width*x, startY + height*y);
}

void GomokuWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouse release";
    Pawn* p = new Pawn(pawnWidth, this);
    p->setColor(Pawn::Color::BLACK);
}
