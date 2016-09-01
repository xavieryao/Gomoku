#ifndef GOMOKUWIDGET_H
#define GOMOKUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QResizeEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include "map.h"

class GomokuWidget : public QWidget
{
    Q_OBJECT

public:
    GomokuWidget(QWidget *parent = 0);
    ~GomokuWidget();


    void setInitColor(Pawn::State color) {initColor = color;}

    void reset();

    QPoint pointForPosition();

    Pawn::State flip(Pawn::State state);


    Pawn::State getInitColor() const;

    QString initColorStr(bool revert = false) const;
    bool isYourTurn() const;

    bool getShowHint() const;
    void setShowHint(bool showHint);

protected:

    void paintEvent(QPaintEvent* event);
    void drawBackground(QPainter* painter, const QRectF &rect);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool hasHeightForWidth() const {return true;}
    int heightForWidth(int w) const {return w;}

signals:
    void move(const QPoint & position);
    void win(bool you);
    void doneMove(bool yourTurn);

private:
    Map mMap;
    qreal mPawnWidth = 0;

    Pawn::State current = Pawn::BLACK;
    Pawn::State initColor = Pawn::NONE;

    bool hasWon();
    bool test(QPoint position);
    QList<QPoint> hint();
    int goodDirections(QPoint position);

    const int HORIZONTAL = 1;
    const int VERTICAL = HORIZONTAL << 1;
    const int CROSS = VERTICAL << 1;
    const int BACK_CROSS = CROSS << 1;

    QList<QPoint> hintList;
    QPixmap hintPix;

    bool mShowHint;

public slots:
    void positionPawn(QPoint position);
    void nextGame();

};

#endif // GOMOKUWIDGET_H
