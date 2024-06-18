/**
 * @file robot.h
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */

#ifndef ROBOTITEM_H
#define ROBOTITEM_H

#include <QGraphicsItem>

class Robot : public QGraphicsItem
{
public:
    Robot(int x, int y, qreal speed, qreal angle, qreal dAngle, qreal omega, qreal range, int direction);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updatePosition();
    bool checkCollisions();
    qreal getSpeed();
    qreal getAngle();
    qreal getDangle();
    qreal getOmega();
    qreal getRange();
    int getDirection();
    void setSpeed(qreal value);
    void setAngle(qreal value);
    void setDangle(qreal value);
    void setOmega(qreal value);
    void setRange(qreal value);
    void setDirection(int value);
    void setVectorOmega();
    void alignAngle();


private:
    qreal m_radius;
    qreal m_speed;
    qreal m_angle;
    qreal m_dAngle;
    qreal m_omega;
    qreal m_range;
    int m_direction;
    qreal to_rotate;
    qreal vector_omega;
    QGraphicsRectItem *m_sightRectItem;
    QPen * rectpen;
};

#endif
