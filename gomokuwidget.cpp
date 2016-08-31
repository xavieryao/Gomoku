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

    // draw hint
    painter.save();
    for (auto& hint: hintList) {
        QPointF point = Map::positionForPoint(hint, this->rect());
        QPen pen(Qt::red);
//        pen.setWidth(5);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(point, pawnWidth, pawnWidth);
    }
    painter.restore();
}

void GomokuWidget::resizeEvent(QResizeEvent *event)
{
    resize(event->size().height(), event->size().height());
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

    if (hasWon()) {
        qDebug() << "winner!" << current;
    }

    // flip
    hintList = hint();
    if (hintList.size() != 0)
        qDebug() << "hint for " << current;
    for (auto& point: hintList) {
        qDebug() << point;
    }
    current = flip(current);

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

QList<QPoint> GomokuWidget::hint()
{
    QList<QPoint> list;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            // iterate through all empty position
            if (mMap[i][j].state() == Pawn::NONE) {
                mMap[i][j].setState(current); // temp

                if (test(QPoint(i, j))) {
                    list.append(QPoint(i, j));
                }
                mMap[i][j].setState(Pawn::NONE);
            }
        }
    }

    return list;
}

int GomokuWidget::goodDirections(QPoint position)
{
    if (position.x() < 0 || position.y() < 0 || position.x() > 14 || position.y() > 14) {
        return 0;
    }
    if (mMap[position.x()][position.y()].state() != current) {
        return 0;
    }


    int result = 0;
    int count, obs;

    // horizontal
    count = 1;
    obs = 0;
    int x = position.x() + 1;
    while (x < 15 && mMap[x][position.y()].state() == current) {
        count ++;
        x++;
    }
    if (x > 14 || mMap[x][position.y()].state() == flip(current)) {
        obs++;
    }
    x = position.x() - 1;
    while (x >= 0 && mMap[x][position.y()].state() == current) {
        count ++;
        x--;
    }
    if (x < 0 || mMap[x][position.y()].state() == flip(current)) {
        obs++;
    }
    if ((count == 3 && obs==0) || (count > 3 && obs <= 1)) {
//        qDebug() << "horizontal";
        result = result | HORIZONTAL;
    }


    // vertical
    count = 1;
    obs = 0;
    int y = position.y() + 1;
    while (y < 15 && mMap[position.x()][y].state() == current) {
        count ++;
        y++;
    }
    if (y > 14 || mMap[position.x()][y].state() == flip(current)) {
        obs++;
    }
    y = position.y() - 1;
    while (y >= 0 && mMap[position.x()][y].state() == current) {
        count ++;
        y--;
    }
    if (y < 0 || mMap[position.x()][y].state() == flip(current)) {
        obs++;
    }
    if ((count == 3 && obs==0) || (count > 3 && obs <= 1)) {
//        qDebug() << "vertical";
        result = result | VERTICAL;
    }

    // ltop <--> rbottom
    count = 1;
    obs = 0;
    x = position.x() + 1;
    y = position.y() +1;
    while (x < 15 && y < 15 && mMap[x][y].state() == current) {
        count ++;
        x++;
        y++;
    }
    if (x > 14 || y > 14 || mMap[x][y].state() == flip(current)) {
        obs++;
    }
    x = position.x() - 1;
    y = position.y() - 1;
    while (x >= 0 && y >= 0 && mMap[x][y].state() == current) {
        count ++;
        x--;
        y--;
    }
    if (x < 0 || y < 0 || mMap[x][y].state() == flip(current)) {
        obs++;
    }
    if ((count == 3 && obs==0) || (count > 3 && obs <= 1)) {
//        qDebug() << "cross";
        result = result | CROSS;
    }

    // rtop <--> lbottom
    count = 1;
    obs = 0;
    x = position.x() + 1;
    y = position.y() - 1;
    while (x < 15 && y >= 0 && mMap[x][y].state() == current) {
        count ++;
        x++;
        y--;
    }
    if (x > 14 || y < 0 || mMap[x][y].state() == flip(current)) {
        obs++;
    }
    x = position.x() - 1;
    y = position.y() + 1;
    while (x >= 0 && y < 15 && mMap[x][y].state() == current) {
        count ++;
        x--;
        y++;
    }
    if (x < 0 || y >14 || mMap[x][y].state() == flip(current)) {
        obs++;
    }
    if ((count == 3 && obs==0) || (count > 3 && obs <= 1)) {

        result = result | BACK_CROSS;
    }
//    qDebug() << "current:" << current << "result:" << result;
    return result;
}

bool GomokuWidget::test(QPoint position)
{
    int result = 0;

    result = result | goodDirections(position);


    int rrr = result;
    unsigned int c =0 ;
    while (rrr >0)
    {
        if((rrr &1) ==1) // 当前位是1
            ++c ; // 计数器加1
        rrr >>=1 ; // 移位
    }
    if (c>=2)
    qDebug() << "current:" << position << "r:" << QString::number(result, 2);
    return c >= 2;
}

Pawn::State GomokuWidget::flip(Pawn::State state) {
    if (state == Pawn::BLACK) {
        return Pawn::WHITE;
    } else {
        return Pawn::BLACK;
    }
}
