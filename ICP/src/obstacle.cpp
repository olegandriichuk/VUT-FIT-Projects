/**
 * @file obstacle.cpp
 * @brief Implementation of the obstacle class
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */
#include "obstacle.h"
#include <QPainter>
#include <QGraphicsScene>

/**
 * @brief Constructs an Obstacle object with the specified parameters.
 *
 * Initializes the obstacle's position and size.
 * @param x Initial x-coordinate of the obstacle
 * @param y Initial y-coordinate of the obstacle
 * @param sideLength Length of one side of the square-shaped obstacle
 */
Obstacle::Obstacle(int x, int y, qreal sideLength) :
    m_sideLength(sideLength)
{
    setPos(x, y);
}

/**
 * @brief Returns the rectangular bounding box of the obstacle for collision detection.
 *
 * @return QRectF Rectangular area occupied by the obstacle.
 */
QRectF Obstacle::boundingRect() const
{
    return QRectF(-m_sideLength / 2, -m_sideLength / 2, m_sideLength, m_sideLength);
}

/**
 * @brief Paints the visual representation of the obstacle on the scene.
 *
 * Draws a blue square to represent the obstacle.
 * @param painter QPainter used for drawing
 * @param option Provides style options for the painting
 * @param widget Widget on which the drawing occurs
 */
void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(Qt::blue);
    painter->drawRect(boundingRect());
}
