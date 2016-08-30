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
    positionPawn(clicked);
}

void GomokuWidget::positionPawn(QPoint position)
{
    mMap[position.x()][position.y()].setState(current);

    qDebug() << "current" << current << "win?" << hasWon();

    // flip
    if (current == Pawn::BLACK) {
        current = Pawn::WHITE;
    } else {
        current = Pawn::BLACK;
    }
    update();

}

bool GomokuWidget::hasWon()
{
    int count = 0;
    // horizontal
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (mMap[i][j].state() == current) {
                count++;
                if (count == 5) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }

    // vertical
    count = 0;
    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 15; i++) {
            if (mMap[i][j].state() == current) {
                count++;
                if (count == 5) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }

    // ltop <-> rbottom
    count = 0;
    for (int i = 0; i < 15; i++) {
        int x = i;
        for (int y = 0; y < 15 && x < 15; y++, x++) {
            if (mMap[x][y].state() == current) {
                count++;
                if (count == 5) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }

    count = 0;
    for (int i = 0; i < 15; i++) {
        int y = i;
        for (int x = 0; x < 15 && y < 15; y++, x++) {
            if (mMap[x][y].state() == current) {
                count++;
                if (count == 5) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }

    // rtop <-> lbottom
    count = 0;
    for (int i = 14; i >= 0; i--) {
        int x = i;
        for (int y = 0; y <=14 && x>=0; y++, x--) {
            if (mMap[x][y].state() == current) {
                count++;
                if (count == 5) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }

    count = 0;
    for (int i = 0; i < 15; i++) {
        int y = i;
        for (int x = 14; x >= 0 && y < 15; y++, x--) {
            if (mMap[x][y].state() == current) {
                count++;
                if (count == 5) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }

    return false;
}
