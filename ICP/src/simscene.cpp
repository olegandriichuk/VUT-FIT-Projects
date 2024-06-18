/**
 * @file simscene.cpp
 * @brief This file defines the simulation scene for managing and displaying robots, controlled robots, and obstacles and scene boundaries.
 *        It contains functions for handling user input, updating the scene, and controlling object attributes.
 * @author Oleg Andriichuk (xandri07)
 * @author Samuel Synek (xsynek04)
 */

#include "simscene.h"
#include "robot.h"
#include "controlledrobot.h"
#include "obstacle.h"

/**
 * @brief Constructs a SimScene object with specified parameters.
 *
 * Initializes the graphics scene, sets up UI connections, and sets the simulation timer.
 * @param ui Pointer to the main window UI
 * @param Width Width of the simulation scene
 * @param Height Height of the simulation scene
 * @param parent Parent object for the graphics scene
 */
SimScene::SimScene(Ui::MainWindow *ui, qreal Width, qreal Height, QObject *parent) :
    QGraphicsScene(parent), m_width(Width), m_height(Height), m_ui(ui)
{
    // Initializing the scene
    initScene();

    // Setting selected item pointers
    selectedItem = NULL;
    selectedRobot = NULL;
    selectedConRobot = NULL;
    changeDirFlagPos();

    // Setting up the timer
    timer = new QTimer(this);
    timer->setInterval(1000 / 10000);

    // Connects the timer and UI elements to their respective slots
    connect(timer, &QTimer::timeout, this, &SimScene::updateScene);
    connect(this, &QGraphicsScene::selectionChanged, this, &SimScene::changeSelection);
    connect(m_ui->StartButton, &QPushButton::clicked, this, &SimScene::startSim);
    connect(m_ui->StopButton, &QPushButton::clicked, this, &SimScene::stopSim);
    connect(m_ui->AddRobotButton, &QPushButton::clicked, this, &SimScene::addRobot);
    connect(m_ui->DeleteRobotButton, &QPushButton::clicked, this, &SimScene::deleteRobot);
    connect(m_ui->AddControlledRobot, &QPushButton::clicked, this, &SimScene::addcontrolledRobot);
    connect(m_ui->DeleteControlledRobot, &QPushButton::clicked, this, &SimScene::deletecontrolledRobot);
    connect(m_ui->AddObstacleButton, &QPushButton::clicked, this, &SimScene::addObstacle);
    connect(m_ui->DeleteObstacleButton, &QPushButton::clicked, this, &SimScene::deleteObstacle);
    connect(m_ui->slider_speed, &QSlider::valueChanged, this, &SimScene::updateSpeed);
    connect(m_ui->slider_angle, &QSlider::valueChanged, this, &SimScene::updateAngle);
    connect(m_ui->slider_rotate, &QSlider::valueChanged, this, &SimScene::updateRotate);
    connect(m_ui->slider_omega, &QSlider::valueChanged, this, &SimScene::updateOmega);
    connect(m_ui->slider_range, &QSlider::valueChanged, this, &SimScene::updateRange);
    connect(m_ui->PositiveButton, &QPushButton::clicked, this, &SimScene::changeDirFlagPos);
    connect(m_ui->NegativeButton, &QPushButton::clicked, this, &SimScene::changeDirFlagNeg);

    connect(timer, &QTimer::timeout, this, &SimScene::updateScene);
}

/**
 * @brief Destructor for SimScene.
 *
 * Deletes all items within the scene and cleans up timer resources.
 */
SimScene::~SimScene()
{
    QList<QGraphicsItem*> itemsToDelete = items();
    for (QGraphicsItem* item : itemsToDelete) {
        delete item;
    }
    delete timer;
}

/**
 * @brief Starts the simulation timer.
 *
 * Resumes or begins the simulation by starting the timer.
 */
void SimScene::startSim()
{
    timer->start();
}

/**
 * @brief Stops the simulation timer.
 *
 * Pauses the simulation by stopping the timer.
 */
void SimScene::stopSim()
{
    timer->stop();
}

/**
 * @brief Deletes the simulation timer.
 *
 * Completely removes the timer, stopping any ongoing simulation updates.
 */
void SimScene::deleteTimer()
{
    delete timer;
}

/**
 * @brief Initializes the scene with borders and sets up the layout.
 *
 * Defines the scene rectangle and adds borders to the simulation.
 */
void SimScene::initScene()
{
    // Set the scene's dimensions
    setSceneRect(0, 0, m_width, m_height);

    // Draw four borders to enclose the simulation area
    QPen borderPen(Qt::black);
    borderPen.setWidth(5);
    QGraphicsLineItem *topBorder = new QGraphicsLineItem(0, 0, m_width, 0);
    topBorder->setPen(borderPen);
    addItem(topBorder);

    QGraphicsLineItem *rightBorder = new QGraphicsLineItem(m_width, 0, m_width, m_height);
    rightBorder->setPen(borderPen);
    addItem(rightBorder);

    QGraphicsLineItem *bottomBorder = new QGraphicsLineItem(m_width, m_height, 0, m_height);
    bottomBorder->setPen(borderPen);
    addItem(bottomBorder);

    QGraphicsLineItem *leftBorder = new QGraphicsLineItem(0, m_height, 0, 0);
    leftBorder->setPen(borderPen);
    addItem(leftBorder);
}

/**
 * @brief Updates the scene with each timer tick.
 *
 * Iterates through all items and updates their positions or attributes if they are robots or controlled robots.
 */
void SimScene::updateScene()
{
    // Retrieve all items to update
    QList<QGraphicsItem*> itemsToUpdate = items();
    foreach (QGraphicsItem* item, itemsToUpdate) {
        // Update position for robots and controlled robots only
        Robot* robot = dynamic_cast<Robot*>(item);
        if (robot) {
            robot->updatePosition();  // Update auto-robot
        }
        ControlledRobot* conrobot = dynamic_cast<ControlledRobot*>(item);
        if (conrobot) {
            conrobot->updatePosition();  // Update controlled robot
        }
    }
}

/**
 * @brief Adds a new auto-robot to the scene with default properties.
 *
 * Creates a new robot and adds it to the scene.
 */
void SimScene::addRobot()
{
    Robot* new_robot = new Robot(250, 250, 30, 45, 75, 30, 50, 1);
    new_robot->setFlag(QGraphicsItem::ItemIsSelectable);
    new_robot->setFlag(QGraphicsItem::ItemIsMovable);
    addItem(new_robot);
    robots.push_back(new_robot);
}

/**
 * @brief Deletes the selected robots from the scene.
 *
 * Iterates through all selected items and removes any robot objects.
 */
void SimScene::deleteRobot()
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        Robot* robot = dynamic_cast<Robot*>(item);
        if (robot) {
            removeItem(robot);
            auto it = std::find(robots.begin(), robots.end(), robot);
            if (it != robots.end()) {
                robots.erase(it);
            }
            delete robot;
        }
    }
}

/**
 * @brief Adds a new obstacle to the scene with default properties.
 *
 * Creates a new obstacle and adds it to the scene.
 */
void SimScene::addObstacle()
{
    Obstacle* new_obstacle = new Obstacle(60, 100, 60);
    new_obstacle->setFlag(QGraphicsItem::ItemIsSelectable);
    new_obstacle->setFlag(QGraphicsItem::ItemIsMovable);
    addItem(new_obstacle);
    obstacles.push_back(new_obstacle);
}

/**
 * @brief Deletes the selected obstacles from the scene.
 *
 * Iterates through all selected items and removes any obstacle objects.
 */
void SimScene::deleteObstacle()
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        if (obstacle) {
            removeItem(obstacle);
            auto it = std::find(obstacles.begin(), obstacles.end(), obstacle);
            if (it != obstacles.end()) {
                obstacles.erase(it);
            }
            delete obstacle;
        }
    }
}

/**
 * @brief Adds a new controlled robot to the scene with default properties.
 *
 * Creates a new controlled robot and adds it to the scene.
 */
void SimScene::addcontrolledRobot()
{
    ControlledRobot* new_robot = new ControlledRobot(100, 100, 5, 0, 5, 30);
    new_robot->setFlag(QGraphicsItem::ItemIsSelectable);
    new_robot->setFlag(QGraphicsItem::ItemIsMovable);
    new_robot->setFlag(QGraphicsItem::ItemIsFocusable);
    addItem(new_robot);
    controllRobots.push_back(new_robot);
}

/**
 * @brief Deletes the selected controlled robots from the scene.
 *
 * Iterates through all selected items and removes any controlled robot objects.
 */
void SimScene::deletecontrolledRobot()
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        ControlledRobot* conrobot = dynamic_cast<ControlledRobot*>(item);
        if (conrobot) {
            removeItem(conrobot);
            auto it = std::find(controllRobots.begin(), controllRobots.end(), conrobot);
            if (it != controllRobots.end()) {
                controllRobots.erase(it);
            }
            delete conrobot;
        }
    }
}

/**
 * @brief Changes the selection state of an item, updating the corresponding UI display.
 *
 * Displays the selected item's speed, angle, omega, and other relevant properties.
 */
void SimScene::changeSelection()
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    if (selectedItems.isEmpty() || selectedItems.count() > 1) {
        return;
    }

    selectedItem = selectedItems.first();

    if (Robot* robt = dynamic_cast<Robot*>(selectedItem)) {
        selectedRobot = robt;
        selectedConRobot = NULL;
        QString speed_string = QString::number(selectedRobot->getSpeed());
        QString angle_string = QString::number(selectedRobot->getAngle());
        QString dangle_string = QString::number(selectedRobot->getDangle());
        QString omega_string = QString::number(selectedRobot->getOmega());
        QString range_string = QString::number(selectedRobot->getRange());
        QString direction_string = selectedRobot->getDirection() == 1 ? "Positive" : "Negative";
        m_ui->v_speed->setText(speed_string);
        m_ui->v_angle->setText(angle_string);
        m_ui->v_dangle->setText(dangle_string);
        m_ui->v_omega->setText(omega_string);
        m_ui->v_range->setText(range_string);
        m_ui->v_direction->setText(direction_string);
    } else if (ControlledRobot* conrobt = dynamic_cast<ControlledRobot*>(selectedItem)) {
        selectedConRobot = conrobt;
        selectedRobot = NULL;
        QString speed_string = QString::number(selectedConRobot->getSpeed());
        QString angle_string = QString::number(selectedConRobot->getAngle());
        QString omega_string = QString::number(selectedConRobot->getOmega());
        QString range_string = QString::number(selectedConRobot->getRange());
        m_ui->v_speed->setText(speed_string);
        m_ui->v_angle->setText(angle_string);
        m_ui->v_omega->setText(omega_string);
        m_ui->v_range->setText(range_string);
    }
}

/**
 * @brief Updates the speed of the selected robot or controlled robot.
 *
 * @param value New speed value.
 */
void SimScene::updateSpeed(int value)
{
    m_ui->v_speed->setText(QString::number(value));
    if (selectedRobot != NULL) {
        selectedRobot->setSpeed(value);
    } else if (selectedConRobot != NULL) {
        selectedConRobot->setSpeed(value);
    }
}

/**
 * @brief Updates the angle of the selected robot or controlled robot.
 *
 * @param value New angle value.
 */
void SimScene::updateAngle(int value)
{
    m_ui->v_angle->setText(QString::number(value));
    if (selectedRobot != NULL) {
        selectedRobot->setAngle(value);
    } else if (selectedConRobot != NULL) {
        selectedConRobot->setAngle(value);
    }
}

/**
 * @brief Updates the rotation angle of the selected robot.
 *
 * @param value New dAngle value.
 */
void SimScene::updateRotate(int value)
{
    m_ui->v_dangle->setText(QString::number(value));
    if (selectedRobot != NULL) {
        selectedRobot->setDangle(value);
        m_ui->v_dangle->setText(QString::number(value));
    } else if (selectedConRobot != NULL) {
        m_ui->v_dangle->setText(QString::number(0));
    }
}

/**
 * @brief Updates the angular velocity (omega) of the selected robot or controlled robot.
 *
 * @param value New omega value.
 */
void SimScene::updateOmega(int value)
{
    m_ui->v_omega->setText(QString::number(value));
    if (selectedRobot != NULL) {
        selectedRobot->setOmega(value);
    } else if (selectedConRobot != NULL) {
        selectedConRobot->setOmega(value);
    }
}

/**
 * @brief Updates the range of the selected robot or controlled robot.
 *
 * @param value New range value.
 */
void SimScene::updateRange(int value)
{
    m_ui->v_range->setText(QString::number(value));
    if (selectedRobot != NULL) {
        selectedRobot->setRange(value);
    } else if (selectedConRobot != NULL) {
        selectedConRobot->setRange(value);
    }
}

/**
 * @brief Updates the rotation direction of the selected robot.
 *
 * @param value New direction value.
 */
void SimScene::updateDirection(int value)
{
    if (selectedRobot != NULL) {
        selectedRobot->setDirection(value);
    }
}

/**
 * @brief Changes the direction flag to positive.
 *
 * Updates the direction of the selected robot and changes the UI display.
 */
void SimScene::changeDirFlagPos()
{
    this->direction_flag = 1;
    m_ui->v_direction->setText("Positive");
    updateDirection(direction_flag);
}

/**
 * @brief Changes the direction flag to negative.
 *
 * Updates the direction of the selected robot and changes the UI display.
 */
void SimScene::changeDirFlagNeg()
{
    this->direction_flag = -1;
    m_ui->v_direction->setText("Negative");
    updateDirection(direction_flag);
}
