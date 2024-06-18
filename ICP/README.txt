Robot Simulation System

Authors
- Oleg Andriichuk (xandri07)
- Samuel Synek (xsynek04)

Overview
This application simulates the movement and interaction of autonomous or manually remote-controlled robots within an environment filled with obstacles and fixed borders. It provides various functionalities for managing the simulation and customizing robot behavior.

How to Use
Starting the Simulation:
Press the Start button to initiate the simulation.

Pausing the Simulation:
Press the Stop button to pause the simulation.

Creating Robots:
Press the Add Robot button or Add Controlled Robot button to add a new autonomous or controlled robot to the simulation. You can edit its position or properties by dragging or selecting the robot within the GUI.

Deleting Robots:
Press the Delete Robot or Delete Controlled Robot button to remove the selected autonomous or controlled robot from the simulation.

Adding Obstacles:
Press the Add Obstacle button to introduce a new obstacle to the simulation. You can adjust its position by dragging it within the GUI.

Deleting Obstacles:
Press the Delete Obstacle button to remove the selected obstacle from the simulation.

Saving the Game:
Press the Save button to save your current configuration to a chosen file.

Loading the Game:
Press the Load button to load your saved configuration from a chosen file. This action pauses the simulation.

Changing Robot's Properties:
Use sliders and push buttons in the GUI to manually adjust the robot's properties. Alternatively, you can edit your configuration file.

Controlling Robots:
Use the W, S, A, D keys to change the controlled robot's states:
- W: Move forward.
- S: Stop moving forward or rotating.
- A: Rotate anti-clockwise.
- D: Rotate clockwise.

Implemented Functionality
- Two types of robots: autonomous robot and remote controlled robot.
- Saving and loading configurations to/from files.
- The ability to change the properties (angle, speed, etc.) of robots during the simulation.
- Ability to stop and start the simulation.
- Ability to change the position of every movable object during the simulation.
- Ability to control the remote controlled robot using the keys W, A, S, D.