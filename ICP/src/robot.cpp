/**
 * @file robot.cpp
 * @brief Implementation of the autonomous robot class
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */
#include "robot.h"
#include "obstacle.h"
#include "controlledrobot.h"
#include <QPainter>
#include <QGraphicsScene>
#include <cmath>

/**
 * @brief Constructs a Robot object with specified parameters.
 *
 * Initializes the robot's position, rotation, speed, sight range, etc.
 * @param x Initial x-coordinate of the robot
 * @param y Initial y-coordinate of the robot
 * @param speed Movement speed of the robot
 * @param angle Current angle of the robot in degrees
 * @param dAngle Angle, that the robot rotates in case of a collision
 * @param omega Angular velocity for the robot's rotation
 * @param range Detection range of the robot
 * @param direction Direction of the angular velocity
 */
Robot::Robot(int x, int y, qreal speed, qreal angle, qreal dAngle, qreal omega, qreal range, int direction) :
    m_speed(speed), m_angle(angle), m_dAngle(dAngle), m_omega(omega), m_range(range), m_direction(direction)
{
    setPos(x, y);

    m_radius = 40;
    to_rotate = 0;
    setVectorOmega();

    rectpen = new QPen();
    rectpen->setColor(Qt::transparent);
    rectpen->setWidth(2);

    m_sightRectItem = new QGraphicsRectItem(this);
    m_sightRectItem->setPen(QPen(Qt::black, 1));
    m_sightRectItem->setBrush(Qt::transparent); // Make the sight rectangle transparent
}

/**
 * @brief Returns the bounding rectangle of the robot for collision detection.
 *
 * @return QRectF The rectangular area occupied by the robot.
 */
QRectF Robot::boundingRect() const
{
    return QRectF(-m_radius/2, -m_radius/2, m_radius, m_radius);
}

/**
 * @brief Paints the visual representation of the robot on the scene.
 *
 * Draws the robot's body and directional indicator.
 * @param painter QPainter used for drawing
 * @param option Provides style options for the painting
 * @param widget Widget on which the drawing occurs
 */
void Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(boundingRect());

    qreal lineLength = m_radius/2;
    qreal radian_angle = m_angle * M_PI / 180;
    qreal x1 = 0;
    qreal y1 = 0;
    qreal x2 = x1 + lineLength * cos(radian_angle);
    qreal y2 = y1 + lineLength * sin(radian_angle);

    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(x1, y1, x2, y2);

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
 * @brief Updates the position and angle of the robot.
 *
 * Moves the robot based on its speed and angle or rotates it in case of a collision.
 */
void Robot::updatePosition()
{
    qreal dx, dy, da, to_rotate_temp, radian_angle;
    dx = dy = 0;

    // Handle rotation if needed
    if(to_rotate != 0){
        da = vector_omega / 1000;
        to_rotate_temp = to_rotate;
        if(da > 0){
            if((to_rotate_temp - da) <= 0){
                m_angle += to_rotate;
                to_rotate = 0;
            }
            else{
                m_angle += da;
                to_rotate -= da;
            }
        }
        else if(da < 0){
            if((to_rotate_temp + da) <= 0){
                m_angle -= to_rotate;
                to_rotate = 0;
            }
            else{
                m_angle += da;
                to_rotate += da;
            }
        }
        return;
    }

    // Move forward if no collisions, otherwise rotate
    if(checkCollisions()){
        radian_angle = m_angle * M_PI / 180;
        dx = m_speed / 200 * cos(radian_angle);
        dy = m_speed / 200 * sin(radian_angle);
    }
    else
    {
        to_rotate = m_dAngle;
    }

    alignAngle();
    setPos(pos().x() + dx, pos().y() + dy);
}

/**
 * @brief Checks for collisions with other items in the scene.
 *
 * Uses the sight rectangle to detect potential collisions.
 * @return bool Returns true if no collisions detected, false otherwise.
 */
bool Robot::checkCollisions()
{
    QList<QGraphicsItem*> collidingItemsList = m_sightRectItem->collidingItems(Qt::IntersectsItemBoundingRect);
    foreach (QGraphicsItem* item, collidingItemsList) {
        if (dynamic_cast<QGraphicsLineItem*>(item) != NULL){
            return false;
        }
        else if(dynamic_cast<Robot*>(item) != NULL && item != this)
            return false;
        else if(dynamic_cast<Obstacle*>(item) != NULL){
            return false;
        }else if(dynamic_cast<ControlledRobot*>(item) != NULL){
            return false;
        }
    }
    return true;
}

/**
 * @brief Returns the current speed of the robot.
 *
 * @return qreal Current speed.
 */
qreal Robot::getSpeed()
{
    return m_speed;
}

/**
 * @brief Returns the current rotation angle of the robot.
 *
 * @return qreal Current rotation angle.
 */
qreal Robot::getAngle()
{
    return m_angle;
}

/**
 * @brief Returns the current angle that the robot rotates in case of a collision.
 *
 * @return qreal Current dAngle value.
 */
qreal Robot::getDangle()
{
    return m_dAngle;
}

/**
 * @brief Returns the current angular velocity of the robot.
 *
 * @return qreal Current omega value.
 */
qreal Robot::getOmega()
{
    return m_omega;
}

/**
 * @brief Returns the detection range of the robot.
 *
 * @return qreal Current detection range.
 */
qreal Robot::getRange()
{
    return m_range;
}

/**
 * @brief Returns the direction of angular velocity.
 *
 * @return int Direction value.
 */
int Robot::getDirection()
{
    return m_direction;
}

/**
 * @brief Sets a new speed value for the robot.
 *
 * @param value New speed value.
 */
void Robot::setSpeed(qreal value)
{
    this->m_speed = value;
}

/**
 * @brief Sets a new rotation angle for the robot.
 *
 * @param value New angle value.
 */
void Robot::setAngle(qreal value)
{
    this->m_angle = value;
}

/**
 * @brief Sets a new dAngle value for the robot.
 *
 * @param value New dAngle value.
 */
void Robot::setDangle(qreal value)
{
    this->m_dAngle = value;
}

/**
 * @brief Sets a new angular velocity value and updates the vector omega.
 *
 * @param value New omega value.
 */
void Robot::setOmega(qreal value)
{
    this->m_omega = value;
    setVectorOmega();
}

/**
 * @brief Sets a new detection range for the robot.
 *
 * @param value New detection range.
 */
void Robot::setRange(qreal value)
{
    this->m_range = value;
}

/**
 * @brief Sets a new direction value for the robot and updates vector omega.
 *
 * @param value New direction value.
 */
void Robot::setDirection(int value)
{
    this->m_direction = value;
    setVectorOmega();
}

/**
 * @brief Updates the vector omega based on angular velocity and direction.
 */
void Robot::setVectorOmega()
{
    this->vector_omega = m_omega * m_direction;
}

/**
 * @brief Aligns the angle of the robot within the [0, 360) range.
 */
void Robot::alignAngle()
{
    m_angle = fmod(m_angle, 360.0);
    if (m_angle < 0)
        m_angle += 360.0;
}
