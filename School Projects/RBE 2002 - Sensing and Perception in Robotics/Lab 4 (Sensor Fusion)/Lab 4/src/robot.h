#pragma once

#include "chassis.h"

/**
 * These are useful for arbitrary navigation.
 */
#define EAST 0
#define NORTH 1
#define WEST 2
#define SOUTH 3

class Robot
{
protected:
    /**
     * We define some modes for you. SETUP is used for adjusting gains and so forth. Most
     * of the activities will run in AUTO. You shouldn't need to mess with these.
     */
    enum ROBOT_CTRL_MODE
    {
        CTRL_TELEOP,
        CTRL_AUTO,
    };
    ROBOT_CTRL_MODE robotCtrlMode = CTRL_AUTO;

    /**
     * robotState is used to track the current task of the robot. You will add new states as 
     * the term progresses.
     */
    enum ROBOT_STATE 
    {
        ROBOT_IDLE,
        ROBOT_TURNING,
        ROBOT_OUTBOUND,
        ROBOT_INBOUND,
        ROBOT_DEAD_RECKONING,
        ROBOT_DRIVING,
        ROBOT_FWD,
        ROBOT_REV,
        ROBOT_SEARCHING,
        ROBOT_APPROACHING,
        ROBOT_LIFTING,
        ROBOT_WEIGHING,
        ROBOT_RAMP,s
    };
    ROBOT_STATE robotState = ROBOT_IDLE;

    /**
     * distanceReading is used to track what type of data manipulation will be used
     * for measuring distance with the ultrasonic sensor.
     */
    enum DISTANCE_READING 
    {
        PRINT_READINGS,
        RUNNING_AVERAGE,
        WINDOWED_AVERAGE,
        MEDIAN_FILTER
    };
    DISTANCE_READING distanceReading = MEDIAN_FILTER;

    /* Define the chassis*/
    Chassis chassis;

    /* Define the ultrasonic rangefinder*/
    //HC_SR04 hc_sr04;

    /* Define the servo*/
    // Servo32U4Pin5 servo;

    /* Define the load cell*/
    // HX711<1, 1> hx711;

    // For managing key presses
    String keyString;

    /**
     * For tracking the motion of the Romi. We'll limit ourselves to a Manhattan grid, with
     * i,j for location and a direction that we're facing.
     */

    /**
     * If you want to do basic navigation, you'll want to track current and target headings
     */
    int8_t currDirection = NORTH;
    int8_t targetDirection = NORTH;
    int8_t iGrid = 0, jGrid = 0;
    int8_t iTarget = 0, jTarget = 0;
    int8_t foundTrash = 0;
    int8_t hasTrash = 0;
    bool trashAlignment = false;
    bool trashToDestination = false;
    float distance = 0;
    float garbageDistance = 0;
    int32_t weight = 0;
    uint16_t SERVO_HIGH = 600;
    uint16_t SERVO_LOW = 1500;
public:
    Robot(void) {keyString.reserve(10);}
    void InitializeRobot(void);
    void RobotLoop(void);

protected:
    /* For managing IR remote key presses*/
    void HandleKeyCode(int16_t keyCode);

    /* For adjusting the servo*/
    void attachServo(void);
    void setServoPosition(int milliseconds);
    void updateServo(void);

    /* For adjusting the load cell*/
    void BeginWeighing(void);

    /* For managing serial input */
    bool CheckSerialInput(void);
    void ParseSerialInput(void);

    /* State changes */    
    void EnterIdleState(void);

    /* Mode changes */
    void EnterTeleopMode(void);
    void EnterAutoMode(void);

    /* Accelerometer ramp driving */
    void BeginRamp(int speed);

    /* High-level navigation methods.*/
    void HandleIntersection(void);
    void BeginTurn(int);
    void HandleTurnComplete(void);
    void HandleDeadReckoningComplete(void);
    void HandleDistanceReading(void);
    void HandleWeightReading(void);
    bool CheckServoAtPosition(void);
    void HandleServoAtPosition(void);
    void HandlePitchAngle(float& pitchAngle, float& headingAngle);

    /* For the Lab 1 Demo */
    void BeginOutAndBack(int8_t destination);
    void BeginToCoordinates(int8_t xCoord, int8_t yCoord);
    void BeginBangBang();
    void BeginUltrasonic(int readings);
};
