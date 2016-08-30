#include "gomokuwidget.h"
#include <QPixmap>
#include <QDebug>
#include <QSizePolicy>
#include "pawn.h"

GomokuWidget::GomokuWidget(QWidget *parent)
    : QWidget(parent)
{
    for (int i = 0; i < 15; i++) {
        QVector<Pawn> vec(15);
        mMap.append(vec);
    }
}

GomokuWidget::~GomokuWidget()
{

}

void GomokuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRectF wholeRect = this->rect();
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
    painter.save();
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 3; i <= 12; i+= 4) {
        for (int j = 3; j <=12; j+=4) {
            QPointF point = Map::positionForPoint(QPoint(i, j), this->rect());
            painter.drawEllipse(point, radius, radius);
        }
    }
    painter.restore();

    // draw pawn
    qreal pawnWidth = blockWidth*0.4;
    if (pawnWidth != mPawnWidth) {
        mPawnWidth = pawnWidth;
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            QPointF point = Map::positionForPoint(QPoint(i,j), this->rect());
            mMap[i][j].paint(&painter, point, mPawnWidth);
        }
    }
}

void GomokuWidget::resizeEvent(QResizeEvent *event)
{
    resize(event->size().height(), event->size().height());
    updateGeometry();
}



void GomokuWidget::mouseReleaseEvent(QMouseEvent *event)
{
//    mMap[3][5].setState(Pawn::BLACK);
    QPoint clicked = Map::pointForPosition(event->pos(), this->rect());
    if (clicked == QPoint(-1, -1)) {
        return;
    }
    if (black) {
        mMap[clicked.x()][clicked.y()].setState(Pawn::BLACK);
        black = !black;
    } else {
        mMap[clicked.x()][clicked.y()].setState(Pawn::WHITE);
        black = !black;
    }
    update();
}
