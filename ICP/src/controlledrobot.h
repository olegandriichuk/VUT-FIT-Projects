/**
 * @file controlledrobot.h
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */
#ifndef CONTROLLEDROBOT_H
#define CONTROLLEDROBOT_H

#include <QGraphicsItem>
#include <QKeyEvent>

class ControlledRobot : public QGraphicsItem {
public:
    ControlledRobot(int x, int y, qreal speed, qreal angle, qreal omega, qreal range);
    qreal getSpeed();
    qreal getAngle();
    qreal getOmega();
    qreal getRange();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    bool checkCollisions();
    void keyPressEvent(QKeyEvent *event) override;
    void updatePosition();
    void updateDirection();
    void setSpeed(qreal value);
    void setAngle(qreal value);
    void setOmega(qreal value);
    void setRange(qreal value);
    void alignAngle();

private:
    qreal m_radius;
    qreal m_speed;
    qreal m_angle;
    qreal m_omega;
    qreal m_range;
    int state;
    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;
    QGraphicsRectItem *m_sightRectItem;
    QPen * rectpen;
};

#endif
