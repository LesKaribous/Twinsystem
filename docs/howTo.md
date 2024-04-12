# How to use TwinSystem

The `src` folder is the base path of the TwinSystem project. All the code is contained in this folder exept the following dependencies :

- TeensyStep (used for stepper motor control)
- Adafruit BNO055 (used for gyroscop and accelerometer sensor)
- Adafruit BusIO (required for the above lib)
- Adafruit Unified sensor (required for the above lib)

## General overview

The Twinsystem project structure is the following : 

```
- src/ 
    - /debug
    - /os
    - /services
    - /utils
    - main.cpp
    - routines.cpp
    - strategy.cpp
    - pin.h
    - routines.h
    - settings.h
    - strategy.h
```

The debug folder contains the Console class used mainly for debugging through the Serial port. This class contains several functions and operators to accelerate the debugging process. (String casting, stream operators, etc.)

The os folder contains everything necessary for the robot to run in a hybrid parallel/sequential fashion. As the Teensy is not multi-threaded, the OS class is responsible for of dispatching the task and managing the tasks handles.

The services folder contains all the services. Service are basically modules that run in the background and that are interopperable between robots and even other microcontroler sharing the same OS architecture.

The utils folder contains all the Utilities such as Timer, Math, Geometry and command interpreter so called "moulinette".

The `main.cpp` is meant to be kept simple. It is responsible for setuping the robot main tasks (routines).
The `routines.cpp` file contains the main programs of the robot
`strategy.cpp` contain our match strategy.

## Routines

```c++
#include "os/console.h"
#include "routines.h"
#include "robot.h"

void setup(){
	Console::init(); // initialize the console (Serial.begin(BAUDRATE))
	Console::setLevel(ConsoleLevel::INFO); // set the console level (verbose, info, error...) 

	os.setRountine(OS::BOOT, onRobotBoot);		        //Execute once						(setup)
	os.setRountine(OS::IDLE, onRobotIdle);		        //Execute during preparation phase 	(loop)
	os.setRountine(OS::RUNNING, onRobotRun);	        //Execute during match				(loop)
	os.setRountine(OS::STOPPED, onRobotStop);	        //Execute after match				(loop)
	os.setRountine(OS::PROGRAM, robotProgram);	        //Execute after match				(loop)
	os.setRountine(OS::IDLE_PROGRAM, robotIdleProgram);	//Execute after match				(loop)
}

void loop(){
	os.run();	//update the services and run the current routine
}
```

As you can see in the previous code snippet, the setup function is essentially setting up the robot state machine by assosciating each state to a predefined routine. The routines are defined in the `routines.h` file.

A routine is essentially a function that must return void and take no parameters. Those rules exist because we are passing these routines as parameters to the os through function pointers. Using `void`  avoid a bit of overhead. For the developers : the standard library `functional` isn't fully supported by our platform. This is why we use old school functions pointers.

As commented in the `main.cpp` file, routines can have different behaviours encoded into the `OS` class in `os.cpp`. 

Classic rountines :

- `BOOT` will execute once.
- `PROGRAM` will executed once during the match. (after starter is pulled)
- `IDLE_PROGRAM` will execute infinitely during the preparation time. (before starter is pulled)
- `STOPPED` will execute infinitely after the match to keep screen running properly (points prediction)

Sub rountines :
- `IDLE` will be executed while the `IDLE_PROGRAM` is executing and after services
- `RUNNING` will be executed while the `PROGRAM` is executing and after services

Main routines are very straightforward, They execute linearly. They key things is that when a blocking function is called inside classic routines, the services and subroutines will be called.

To say that a function may be blocking to the OS we use a custom keyword `async` just in front of them like this :

``` c++

void mySubroutine(){
    Console::println("Still running");
}

void myRoutine(){
    Console::println("Start routine");
    async myBlockingJob();
    Console::println("end routine");
}

void setup(){
	Console::init();
	Console::setLevel(ConsoleLevel::INFO);
	os.setRountine(OS::RUNNING, mySubroutine); //Execute during match	(loop)
	os.setRountine(OS::PROGRAM, myRoutine);	   //Execute after match	(loop)
}

void loop(){
	os.run();	//update the services and run the current routine
}

```

Note that to be async, the blocking function must return a Job reference. ex:
```
Job& myBlockingTask()
```
The exact implementation and usage won't be discussed here and may a bit next year to better match the OS architecture. However, the reader in encouraged to reach out or to read the implementation of the Motion service.

## Console

The Console library provides static methods used for debug purposes. You can use the Console like this:

```c++
Console::println("Hello World");

Console::print("Distance :"); // prints string
Console::println(myVar); //print almost all king to data...

Console::line(); //Draw an horizontal line in the terminal
Console::prettyPrint(code); //pretty prints code with line numbers

//Plot value using the Teleplot syntax 
Console::plot("myDataName", Value); //https://marketplace.visualstudio.com/items?itemName=alexnesnes.teleplot

```

The Console class has also stream based methods

```c++
Console::info();
Console::warn();
Console::error();
Console::trace();
Console::success();

```

You can used them as follows :

```c++
Console::info() << "My data is :" << myVar << " mm" << Console::endl;
```

This would output : 

```
My data is :0 mm
```

According to the current Console level of verbose, the stream methods may be ignored.
For instance if you call `Console::info() << "Hello" << Console::endl;` and the Console level is `Console::ERROR`, nothing will be output since info is lower than error level.

The available levels are : 

- VERBOSE
- INFO
- SUCCESS
- WARNING
- CRITICAL
- DISABLED

## OS

The os is relatively easy to used once you have set up the rountines. It is handling everything automatically and Thus there is not much interaction with it during hot development phase. (That's the plan at least)

The OS class exist using a singleton. You can acces it as soon as you are including the `os.h` file.
Just call `os.theMethod();

The following methods exist :
```c++
    os.isBusy(); //return true if a task is running or paused
    os.execute(job); //start a paralell task (non blocking)
    os.execute(job, false); //start a task and execute it before return (blocking)

    os.wait(duration); //wait for a given duration (services and parallel routines will run)
    os.waitUntil(Job& job, bool runasync = false); //wait for a given job to end or to be canceled (services and parallel routines will run)

    void setRountine(SystemState::THE_STATE, myRoutine); //Set a routine
    void setState(SystemState::THE_STATE); //Set the current state

    void enable(ServiceID); //enable a service
    void disable(ServiceID); //disable a service
    void attachService(Service*); //attach a service (and enable it)
    bool hasService(ServiceID) const; //return true if OS has the service
    bool statusService(ServiceID serviceID) const; //return status (enabled or not) of the service
    bool debug(ServiceID); // return true if service should debug
    void toggleDebug(ServiceID s);// activate debug of the service (Console will print mesage from this service)
```

