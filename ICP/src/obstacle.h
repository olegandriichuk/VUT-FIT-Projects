/**
 * @file obstacle.h
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */
#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <QGraphicsItem>

class Obstacle : public QGraphicsItem
{
public:
    Obstacle(int x, int y, qreal sideLength);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
private:
    qreal m_sideLength;
};

#endif // OBSTACLE_H
