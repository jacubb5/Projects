#include "robot.h"
#include <IRdecoder.h>
#include "serial_comm.h"

void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();

    /**
     * Initialize the IR decoder. Declared extern in IRdecoder.h; see robot-remote.cpp
     * for instantiation and setting the pin.
     */
    decoder.init();
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}

/**
 * The main loop for your robot. Process both synchronous events (motor control),
 * and asynchronous events (IR presses, distance readings, etc.).
*/
void Robot::RobotLoop(void) 
{
    /**
     * Handle any IR remote keypresses.
     */
    int16_t keyCode = decoder.getKeyCode();
    if(keyCode != -1) HandleKeyCode(keyCode);

    /**
     * Run the chassis loop, which handles low-level control.
     */
    if(chassis.ChassisLoop())
    {
        // You can put post motor update tasks here
    }
    
    // Check for Chassis events
    if(chassis.CheckIntersection()) HandleIntersection();
    if(chassis.CheckTurnComplete()) HandleTurnComplete();
    if(chassis.CheckDeadReckoningComplete()) HandleDeadReckoningComplete();

    /** 
     * The code below allows you to adjust gains using the Serial Monitor.
     * CheckSerialInput() returns true when it gets a complete string, which is
     * denoted by a newline character ('\n'). 
     * 
     * WARNING: Be sure to set your Serial Monitor to append a newline!!!!
     * 
     * When you're done tuning, you can comment out or just remove the line below 
     * to save ~1.5k of program memory.
     */
    // if(CheckSerialInput()) ParseSerialInput();
}