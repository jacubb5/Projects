#pragma once

#include "chassis.h"
#include <openmv.h>
#include <event_timer.h>

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
        ROBOT_DRIVE_TO_POINT,
        ROBOT_SEARCHING,
        ROBOT_APPROACHING,
        ROBOT_TURNING
    };
    ROBOT_STATE robotState = ROBOT_IDLE;

    /* Define the chassis*/
    Chassis chassis;

    // For managing key presses
    String keyString;

    /**
     * For tracking current pose and the destination.
     */
    Pose currPose;
    Pose destPose;

    /**
     * Error values for IK
     */
    float xError;
    float yError;
    float thetaError;

    /**
     * Control values for IK
     */
    float KP_U = 0.7;
    float KP_V = 0.0; // Not used
    float KP_OMEGA = 15.0;

    /**
     * Control values for Apriltag alignment
     */
    float distError = 0;
    float turnError = 0;
    float KP_DIST = 0.18;
    float KP_THETA = 0.4;
    
    /* Define the camera and tag data */
    OpenMV camera;
    AprilTagDatum tag;

    /* Define a timer for Apriltag timeout */
    EventTimer tagTimer;
    int tagTimeout = 500;

    /* Dead reckoning timer */
    EventTimer turnTimer;

public:
    Robot(void) {keyString.reserve(10);}
    void InitializeRobot(void);
    void RobotLoop(void);

protected:
    /* For managing IR remote key presses*/
    void HandleKeyCode(int16_t keyCode);

    /* State changes */    
    void EnterIdleState(void);

    /* Mode changes */
    void EnterTeleopMode(void);
    void EnterAutoMode(void);
    void EnterSearchingState(bool positiveSpin);
    void EnterApproachingState(void);
    void EnterTurningState(bool positiveSpin);

    // /* Navigation methods.*/
    void UpdatePose(const Twist& u);
    void SetDestination(const Pose& destination);
    void DriveToPoint(void);
    void AlignWithTag(void);
    bool CheckReachedDestination(void);
    bool CheckAprilTag(void);
    bool CheckTagTimerExpired(void);
    bool CheckApproachComplete(void);
    bool CheckTurnTimerExpired(void);
    void HandleDestination(void);
    void HandleAprilTag(void);
    void HandleTagTimerExpired(void);
    void HandleApproachComplete(void);
    void HandleTurnTimerExpired(void);
};
