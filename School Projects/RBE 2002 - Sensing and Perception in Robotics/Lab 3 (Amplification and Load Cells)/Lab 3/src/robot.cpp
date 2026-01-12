#include "robot.h"
#include <IRdecoder.h>
#include "serial_comm.h"
#include <Arduino.h>
#include <HC-SR04.h>
#include <Sharp-IR.h>
#include <HX711.h>

HC_SR04 hc_sr04(11, 0); // TODO: Choose your pins. ECHO first, then TRIG.
HX711<30, 1> hx711;


void ISR_HC_SR04(void)
{
    hc_sr04.ISR_echo();
}

void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();
    hc_sr04.init(ISR_HC_SR04);
    attachServo();
    hx711.Init();
    hx711.Wakeup();
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
    
    // Update servo position
    updateServo();    

    // Check for Chassis events
    if(chassis.CheckIntersection()) HandleIntersection();
    if(chassis.CheckTurnComplete()) HandleTurnComplete();
    if(chassis.CheckDeadReckoningComplete()) HandleDeadReckoningComplete();
    if(hc_sr04.getDistance(distance)) HandleDistanceReading();
    if(hx711.GetReading(weight)) HandleWeightReading();
    if(CheckServoAtPosition()) HandleServoAtPosition();

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
