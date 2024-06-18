/**
 * @file simscene.h
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */
#ifndef SIMSCENE_H
#define SIMSCENE_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QTimer>
#include <string>
#include <vector>
#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include "robot.h"
#include "controlledrobot.h"
#include "obstacle.h"
#include "ui_mainwindow.h"

class SimScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SimScene(Ui::MainWindow* ui,qreal Width, qreal Height, QObject *parent = NULL);

    void updateScene();

    void updateSpeed(int value);
    void updateAngle(int value);
    void updateRotate(int value);
    void updateOmega(int value);
    void updateRange(int value);
    void updateDirection(int value);
    void startSim();
    void stopSim();
    void addRobot();
    void deleteRobot();
    void addcontrolledRobot();
    void deletecontrolledRobot();
    void addObstacle();
    void deleteObstacle();
    void saveScene();
    void loadScene();
    void changeSelection();
    void deleteTimer();
    void changeDirFlagNeg();
    void changeDirFlagPos();

    ~SimScene();

    std::vector <Obstacle*> obstacles;
    std::vector <Robot*> robots;
    std::vector <ControlledRobot*> controllRobots;

private:
    void initScene();
    qreal m_width;
    qreal m_height;
    QTimer * timer;
    QGraphicsItem * selectedItem;
    Robot * selectedRobot;
    ControlledRobot * selectedConRobot;
    int direction_flag;
    Ui::MainWindow *m_ui;
};

#endif
