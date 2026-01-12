#include "robot.h"
#include <IRdecoder.h>


void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();
    
    /**
     * Initialize the I2C communication between the Romi and the camera
     */
    Wire.begin();
    Wire.setClock(100000ul);
    
    /**
     * Initialize the IR decoder. Declared extern in IRdecoder.h; see robot-remote.cpp
     * for instantiation and setting the pin.
     */
    decoder.init();
    

    pinMode(13, OUTPUT);
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();
    digitalWrite(13, LOW);

    Serial.println("-> IDLE");
    Serial.println(millis());
    robotState = ROBOT_IDLE;
}

void Robot::EnterSearchingState(bool positiveSpin)
{
    if (positiveSpin) {
        chassis.SetWheelSpeeds(-5, 5);
    } else {
        chassis.SetWheelSpeeds(5, -5);
    }

    digitalWrite(13, LOW);

    Serial.println("<-> SEARCHING");
    robotState = ROBOT_SEARCHING;
}

void Robot::EnterApproachingState(void)
{
    digitalWrite(13, HIGH);

    Serial.println("--> APPROACHING");
    robotState = ROBOT_APPROACHING;
}

void Robot::EnterTurningState(bool positiveSpin)
{
    turnTimer.start(2000);
    if (positiveSpin)
    {
        chassis.SetWheelSpeeds(-5.5, 5.5);
    }
    else
    {
        chassis.SetWheelSpeeds(5.5, -5.5);
    }

    Serial.println("--> TURNING");
    robotState = ROBOT_TURNING;
}

bool prevCam = false;
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
    Twist velocity;
    if(chassis.ChassisLoop(velocity))
    {
        // We do FK regardless of state
        UpdatePose(velocity);

        /**
         * Here,/ we break with tradition and only call these functions if we're in the 
         * DRIVE_TO_POINT state. CheckReachedDestination() is expensive, so we don't want
         * to do all the maths when we don't need to.
         * 
         * While we're at it, we'll toss DriveToPoint() in, as well.
         */ 
        if(robotState == ROBOT_DRIVE_TO_POINT)
        {
            DriveToPoint();
            if(CheckReachedDestination()) HandleDestination();
        }
        if(robotState == ROBOT_APPROACHING)
        {
            AlignWithTag();
            if(CheckApproachComplete()) HandleApproachComplete();
        }
    }
    
    if(CheckAprilTag()) HandleAprilTag();
    if(CheckTagTimerExpired()) HandleTagTimerExpired();
    if(turnTimer.CheckExpired()) HandleTurnTimerExpired();
}

