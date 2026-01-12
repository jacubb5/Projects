#pragma once

#include <Arduino.h>
#include <event_timer.h>
#include <LineSensor.h>
#include <Arduino.h>
#include <HC-SR04.h>
#include "chassis-params.h"
#include "LSM6.h"


class Chassis
{
protected:
    enum CHASSIS_STATE { CHASSIS_IDLE, CHASSIS_LINING, CHASSIS_DEAD_RECKONING, CHASSIS_TURNING, CHASSIS_HEADING_DRIVE };
    CHASSIS_STATE chassisState = CHASSIS_IDLE;
    
    LSM6 imu;

    /**
     * You can change the control loop period, but you should use multiples of 4 ms to 
     * avoid rounding errors.
     */
    const uint16_t CONTROL_LOOP_PERIOD_MS = 4;

    /**
     * loopFlag is used to tell the program when to update. It is set when Timer4
     * overflows (see InitializeMotorControlTimer). Some of the calculations are too
     * lengthy for an ISR, so we set a flag and use that to key the calculations.
     * 
     * Note that we use in integer so we can see if we've missed a loop. If loopFlag is
     * more than 1, then we missed a cycle.
     */
    static uint8_t loopFlag;

    /* Line sensor */
    LineSensor lineSensor;
    float lineSpeed = 0;

    /* Timer for dead reckoning */
    EventTimer turnTimer;
    EventTimer deadReckoningTimer;

    /* IMU Headings */
    float headingDriveSpeed = 0;
    float heading = 0;

public:
    Chassis(void) {}
    void InititalizeChassis(void)
    {
        InitializeMotorControlTimer();
        InitializeMotors();
        imu.init();

        // The line sensor elements default to INPUTs, but we'll initialize anyways, for completeness
        lineSensor.Initialize();
    }

    // For setting motor control gains; Kd is unneeded
    void SetMotorKp(float kp);
    void SetMotorKi(float ki);
    void SetMotorKd(float kd);

    /* Where the bulk of the work for the motors gets done. */
    bool ChassisLoop(void);

    /* Needed for managing motors. */
    static void Timer4OverflowISRHandler(void);

public:
    /**
     * A pair of basic navigation routines. If you want to consolidate into
     * SetTwist(), you may do so, but you won't need that until the second
     * half of the term.
     */
    void DriveAt(float fwdSpeedCMsec);
    void TurnAt(float angVelRPM);

    /* Not yet implemented. */
    void SetTwist(float fwdSpeedCMsec, float angVelRPM);

    /**
     * A utility function for converting robot speed to wheel speed. Left 
     * public so that you can test more easily.
     */
    void SetWheelSpeeds(float, float);

    void Stop(void) {SetMotorEfforts(0, 0); chassisState = CHASSIS_IDLE; }

    /**
     * IMU reading
     */
    bool CheckIMU(float& currentAngleEst, float& currenHeadingEst);

    /**
     * Teleplot printing helper function
     */
    void TeleplotPrint(const char* var, float value);

    /**
     * Line following routines.
     */
    void EnterLineFollowing(float lineSpeed);
    void LineFollowingUpdate(void);
    bool CheckIntersection(void) { return lineSensor.CheckIntersection(); }
    bool CheckOverLine(void) { return lineSensor.CheckOverLine(); }
    /**
     * Turning routines
     */
    void BeginTurn(uint8_t);
    bool CheckTurnComplete(void);
    bool CheckDeadReckoningComplete(void);
    void BeginDeadReckoning(int distance);
    /**
     * IMU heading routines
     */
    void BeginHeadingDrive(float speed);
    void HeadingUpdate(void);

protected:
    /**
     * Initialization and Setup routines
     */
    void InitializeMotorControlTimer(void);
    void InitializeMotors(void);
    
    void UpdateMotors(void);
    void SetMotorEfforts(int16_t, int16_t);
};
