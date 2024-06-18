/**
 * @file mainwindow.cpp
 * @brief This file defines the main window of the application and provides the functionality
 *        for controlling and displaying the simulated robots, controlled robots, and obstacles.
 *        It includes features to save and load scenes.
 * @author Oleg Andrichuk(xandri07)
 * @author Samuel Synek (xsynek04)
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "robot.h"
#include "controlledrobot.h"
#include "obstacle.h"
#include "simscene.h"
#include <sstream>
#include <iostream>
#include <QString>
#include <QFileDialog>

/**
 * @brief Constructs the MainWindow object.
 *
 * Initializes the user interface, connects UI elements to their respective event handlers,
 * sets up sliders for simulation control, and creates the simulation scene.
 * @param parent Parent widget
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Disable scroll bars in the scene window
    ui->SceneWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->SceneWindow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Connect buttons to their respective event handlers
    connect(ui->SaveButton, &QPushButton::clicked, this, &MainWindow::saveScene);
    connect(ui->LoadButton, &QPushButton::clicked, this, &MainWindow::loadScene);

    // Setup sliders for speed, angle, rotation, omega, and range
    ui->slider_speed->setMinimum(0);
    ui->slider_speed->setMaximum(100);
    ui->slider_angle->setMinimum(0);
    ui->slider_angle->setMaximum(360);
    ui->slider_rotate->setMinimum(0);
    ui->slider_rotate->setMaximum(360);
    ui->slider_omega->setMinimum(0);
    ui->slider_omega->setMaximum(100);
    ui->slider_range->setMinimum(1);
    ui->slider_range->setMaximum(500);

    // Initialize the simulation scene
    qreal sceneWidth = ui->SceneWindow->width() - 50;
    qreal sceneHeight = ui->SceneWindow->height() - 50;
    scene = new SimScene(ui, sceneWidth, sceneHeight, this);
    ui->SceneWindow->setScene(scene);
    ui->SceneWindow->setRenderHint(QPainter::Antialiasing);
}

/**
 * @brief Destructor for the MainWindow class.
 *
 * Cleans up dynamically allocated memory.
 */
MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

/**
 * @brief Loads a simulation scene from a file selected via a file dialog.
 *
 * Clears the current scene and loads robots, controlled robots, and obstacles and their properties
 * from the specified file.
 */
void MainWindow::loadScene() {
    scene->stopSim();
    QString fileName = QFileDialog::getOpenFileName(this, "Select File", "", "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty()) {
        qDebug() << "No file selected.";
        return;
    }

    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        qDebug() << "Failed to open file:" << fileName;
        return;
    }

    std::string line;
    std::getline(file, line);

    // Clear the current scene
    for (auto robot : scene->robots) delete robot;
    for (auto obstacle : scene->obstacles) delete obstacle;
    for (auto controlledRobot : scene->controllRobots) delete controlledRobot;
    scene->robots.clear();
    scene->obstacles.clear();
    scene->controllRobots.clear();

    // Read the data from the file and create objects
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type, skip;
        qreal posX, posY, speed, angle, dAngle, omega, range, direction;

        iss >> type >> skip;
        iss >> posX >> skip;
        iss >> posY >> skip;

        if (type == "Robot") {
            iss >> speed >> skip;
            iss >> angle >> skip;
            iss >> dAngle >> skip;
            iss >> omega >> skip;
            iss >> range >> skip;
            iss >> direction;
            Robot* robot = new Robot(posX, posY, speed, angle, dAngle, omega, range, direction);
            robot->setFlag(QGraphicsItem::ItemIsSelectable);
            robot->setFlag(QGraphicsItem::ItemIsMovable);
            scene->addItem(robot);
            scene->robots.push_back(robot);
        } else if (type == "Obstacle") {
            Obstacle* new_obstacle = new Obstacle(posX, posY, 60);
            new_obstacle->setFlag(QGraphicsItem::ItemIsSelectable);
            new_obstacle->setFlag(QGraphicsItem::ItemIsMovable);
            scene->addItem(new_obstacle);
            scene->obstacles.push_back(new_obstacle);
        } else if (type == "Controlled_Robot") {
            iss >> speed >> skip;
            iss >> angle >> skip;
            iss >> omega >> skip;
            iss >> range;
            ControlledRobot* new_ControlledRobot = new ControlledRobot(posX, posY, speed, angle, omega, range);
            new_ControlledRobot->setFlag(QGraphicsItem::ItemIsSelectable);
            new_ControlledRobot->setFlag(QGraphicsItem::ItemIsMovable);
            new_ControlledRobot->setFlag(QGraphicsItem::ItemIsFocusable);
            scene->addItem(new_ControlledRobot);
            scene->controllRobots.push_back(new_ControlledRobot);
        }
    }

    file.close();
    qDebug() << "Data loaded successfully from file";
}

/**
 * @brief Saves the current simulation scene to a file selected via a file dialog.
 *
 * Writes the positions and properties of robots, controlled robots, and obstacles to the specified file.
 */
void MainWindow::saveScene() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty()) {
        qDebug() << "No file selected.";
        return;
    }

    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        qDebug() << "Failed to open file:" << fileName;
        return;
    }

    file << "Object-type, position_x, position_y, speed(robot), angle(robot), dangle(robot), omega(robot), range(robot), direction(robot)" << std::endl;

    for (const auto& obstacle : scene->obstacles) {
        file << "Obstacle   , " << obstacle->pos().x() << ", " << obstacle->pos().y() << std::endl;
    }

    for (const auto& robot : scene->robots) {
        file << "Robot      , " << robot->pos().x() << ", " << robot->pos().y() << ", " << robot->getSpeed() << ", " << robot->getAngle() << ", " << robot->getDangle() << ", " << robot->getOmega() << ", " << robot->getRange() << ", " << robot->getDirection() << std::endl;
    }

    for (const auto& controlledRobot : scene->controllRobots) {
        file << "Controlled_Robot , " << controlledRobot->pos().x() << ", " << controlledRobot->pos().y() << ", " << controlledRobot->getSpeed() << ", " << controlledRobot->getAngle() << ", " << controlledRobot->getOmega() << ", " << controlledRobot->getRange() << std::endl;
    }

    file.close();
    qDebug() << "Data saved successfully to file";
}
