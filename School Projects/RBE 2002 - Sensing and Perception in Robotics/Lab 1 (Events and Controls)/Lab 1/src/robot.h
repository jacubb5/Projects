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
        ROBOT_DRIVING,s
    };
    ROBOT_STATE robotState = ROBOT_IDLE;

    /* Define the chassis*/
    Chassis chassis;

    /* To add later: rangefinder, camera, etc.*/

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
    
public:
    Robot(void) {keyString.reserve(10);}
    void InitializeRobot(void);
    void RobotLoop(void);

protected:
    /* For managing IR remote key presses*/
    void HandleKeyCode(int16_t keyCode);

    /* For managing serial input */
    bool CheckSerialInput(void);
    void ParseSerialInput(void);

    /* State changes */    
    void EnterIdleState(void);

    /* Mode changes */
    void EnterTeleopMode(void);
    void EnterAutoMode(void);

    /* High-level navigation methods.*/
    void HandleIntersection(void);
    void BeginTurn(int);
    void BeginDeadReckoning(int);
    void HandleTurnComplete(void);
    void HandleDeadReckoningComplete(void);

    /* For the Lab 1 Demo */
    void BeginOutAndBack(int8_t destination);
    void BeginToCoordinates(int8_t xCoord, int8_t yCoord);
};
