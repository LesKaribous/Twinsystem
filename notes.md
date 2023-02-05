# Notes

## Features
    - [ ] Modular architecture
    - [ ] Program with several actions
    - [ ] Obstacle avoidance
    - [ ] Advanced movement strategies
    - [ ] 


## Random advices : 

Break down the competition tasks into smaller, manageable sub-tasks. For example, if the task is to pick up and transport an object, you may need to develop separate functionality for object detection, grasping, and movement.

Focus on developing a robust control system that can handle the various sub-tasks and coordinate the actions of the robot. This will likely involve some form of state machine or decision-making algorithm.

Use sensors to provide the robot with information about its environment and the objects within it. This can include cameras, lidar, and other sensors depending on the specific tasks and the rules of the competition.

Test and refine your robot's performance in simulated environments before moving on to physical testing. This will allow you to quickly identify and fix any issues without risking damage to your robot.

Finally, make sure you pay attention to the rules of the competition and that your robot is compliant with them



System : 

Initialize all the subsystems and objects that the System class will use. This includes things like sensors, actuators, and other objects you've created to handle specific tasks or functionality.

Write a main loop that will execute continuously while the robot is running. The main loop should include code to read sensor data, update the robot's state, and control the actuators.

Use a state machine or decision-making algorithm to determine the robot's next action based on the sensor data and the current state of the robot.

Implement a way for the System class to communicate with other objects and classes in your code, this will allows the robot to perform tasks and make decisions

Test the System class thoroughly in simulated environments and with the physical robot to ensure that it is working correctly and that the robot can accomplish the competition tasks.


Feature ? 
UpdateSensors(): A method to read and update the data from the robot's sensors. This could include things like reading data from a lidar sensor, or processing images from a camera.

UpdateActuators(): A method to control the robot's actuators based on the sensor data and the current state of the robot. This could include things like controlling the speed of the motors or the position of a robotic arm.

DecisionMaking(): A method to handle the decision making process of the robot. This could include things like determining the robot's next action based on the sensor data and the current state of the robot.

CheckBattery(): A method to check the battery level and handle the case where the battery is low.

CheckEmergency(): A method to check for emergency situations, such as collisions or unexpected situations and handle them.

SendTelemetry(): A method to send telemetry data to a remote system, this could include sensor data, states of the robot, battery level and so on.

ReceiveCommands(): A method to receive commands from a remote system, this could include commands to change the state of the robot or to run specific tasks.

CheckTime(): A method to check the time remaining of the match and handle the case where the match is finished