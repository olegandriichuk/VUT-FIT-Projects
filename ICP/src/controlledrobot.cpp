/**
 * @file controlledrobot.cpp
 * @brief Implementation of the controlled robot class
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */
#include "controlledrobot.h"
#include "obstacle.h"
#include <QPainter>
#include <QGraphicsScene>
#include <cmath>
#include "robot.h"

/**
 * @brief Constructs a ControlledRobot object with specified parameters.
 *
 * Initializes the robot's position, speed, angle, and sight range.
 * @param x Initial x-coordinate of the controlled robot
 * @param y Initial y-coordinate of the controlled robot
 * @param speed Movement speed of the controlled robot
 * @param angle Current angle of the controlled robot in degrees
 * @param omega Angular velocity for the robot's rotation
 * @param range Detection range of the controlled robot
 */
ControlledRobot::ControlledRobot(int x, int y, qreal speed, qreal angle, qreal omega, qreal range):
    m_speed(speed),  m_angle(angle), m_omega(omega), m_range(range)
{
    setPos(x, y);

    m_radius = 40;
    state = 0;

    rectpen = new QPen();
    rectpen->setColor(Qt::transparent);
    rectpen->setWidth(2);

    m_sightRectItem = new QGraphicsRectItem(this);
    m_sightRectItem->setPen(QPen(Qt::black, 1));
    m_sightRectItem->setBrush(Qt::transparent); // Make the sight rectangle transparent
}

/**
 * @brief Returns the current speed of the controlled robot.
 *
 * @return qreal Current speed.
 */
qreal ControlledRobot::getSpeed(){
    return m_speed;
}

/**
 * @brief Returns the current rotation angle of the controlled robot.
 *
 * @return qreal Current angle.
 */
qreal ControlledRobot::getAngle(){
    return m_angle;
}

/**
 * @brief Returns the current angular velocity of the controlled robot.
 *
 * @return qreal Current omega value.
 */
qreal ControlledRobot::getOmega(){
    return m_omega;
}

/**
 * @brief Returns the detection range of the controlled robot.
 *
 * @return qreal Current detection range.
 */
qreal ControlledRobot::getRange(){
    return m_range;
}

/**
 * @brief Returns the rectangular area occupied by the controlled robot.
 *
 * @return QRectF Bounding rectangle area.
 */
QRectF ControlledRobot::boundingRect() const
{
    return QRectF(-m_radius/2, -m_radius/2, m_radius, m_radius);
}

/**
 * @brief Paints the visual representation of the controlled robot on the scene.
 *
 * Draws the controlled robot's body and directional indicator.
 * @param painter QPainter used for drawing
 * @param option Provides style options for the painting
 * @param widget Widget on which the drawing occurs
 */
void ControlledRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::green);
    painter->drawEllipse(boundingRect());

    qreal lineLength = m_radius/2;
    double radian_angle = m_angle * M_PI / 180;
    qreal x1 = 0;
    qreal y1 = 0;
    qreal x2 = x1 + lineLength * cos(radian_angle);
    qreal y2 = y1 + lineLength * sin(radian_angle);

    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(x1,y1,x2,y2);

    qreal sightWidth = m_radius;
    qreal sightLength = m_radius / 2 + m_range;
    qreal rectX = x1;
    qreal rectY = -sightWidth / 2;
    QRectF sightRect(rectX, rectY, sightLength, sightWidth);

    m_sightRectItem->setPen(*rectpen);
    m_sightRectItem->setRect(sightRect);
    m_sightRectItem->setRotation(m_angle);
}

/**
 * @brief Checks for collisions using the controlled robot's sight rectangle.
 *
 * Uses dynamic casting to determine the type of the colliding item.
 * @return bool Returns true if no collisions detected, false otherwise.
 */
bool ControlledRobot::checkCollisions()
{
    QList<QGraphicsItem*> collidingItemsList = m_sightRectItem->collidingItems(Qt::IntersectsItemBoundingRect);
    foreach (QGraphicsItem* item, collidingItemsList) {
        if (dynamic_cast<QGraphicsLineItem*>(item) != NULL){
            return false;
        }
        else if(dynamic_cast<ControlledRobot*>(item) != NULL && item != this)
            return false;
        else if(dynamic_cast<Obstacle*>(item) != NULL){
            return false;
        }else if(dynamic_cast<Robot*>(item) != NULL){
            return false;
        }
    }
    return true;
}

/**
 * @brief Handles keyboard input to control the movement of the robot.
 *
 * Changes the state based on the input key.
 * @param event Key event containing the key that was pressed.
 */
void ControlledRobot::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W:
        state = 1;
        break;
    case Qt::Key_S:
        state = 0;
        break;
    case Qt::Key_A:
        state = 2;
        break;
    case Qt::Key_D:
        state = 3;
        break;
    }
}

/**
 * @brief Updates the position and angle of the controlled robot.
 *
 * Moves forward if no collisions, rotates left or right based on user input.
 */
void ControlledRobot::updatePosition() {

    qreal radian_angle, dx, dy;

    switch (state){
    case 0:
        break;
    case 1:
        if(checkCollisions()){
            // no collision
            radian_angle = m_angle * M_PI / 180;
            dx = m_speed / 200 * cos(radian_angle);
            dy = m_speed / 200 * sin(radian_angle);
            setPos(pos().x() + dx, pos().y() + dy);
            break;

        }
        else
        {
            state = 0;
            break;
        }
    case 2:
        m_angle -= m_omega / 1000;
        break;

    case 3:
        m_angle += m_omega / 1000;
        break;
    }
    alignAngle();
}

/**
 * @brief Sets a new speed value for the controlled robot.
 *
 * @param value New speed value.
 */
void ControlledRobot::setSpeed(qreal value)
{
    this->m_speed = value;
}

/**
 * @brief Sets a new rotation angle for the controlled robot.
 *
 * @param value New angle value.
 */
void ControlledRobot::setAngle(qreal value)
{
    this->m_angle = value;
}

/**
 * @brief Sets a new angular velocity value for the controlled robot.
 *
 * @param value New omega value.
 */
void ControlledRobot::setOmega(qreal value)
{
    this->m_omega = value;
}

/**
 * @brief Sets a new detection range for the controlled robot.
 *
 * @param value New detection range value.
 */
void ControlledRobot::setRange(qreal value)
{
    this->m_range = value;
}

/**
 * @brief Aligns the angle of the controlled robot within the [0, 360) range.
 */
void ControlledRobot::alignAngle()
{
    m_angle = fmod(m_angle, 360.0);
    if (m_angle < 0)
        m_angle += 360.0;
}
