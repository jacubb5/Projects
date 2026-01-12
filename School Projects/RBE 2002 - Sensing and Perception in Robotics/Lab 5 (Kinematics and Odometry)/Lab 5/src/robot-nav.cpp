/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"

void Robot::UpdatePose(const Twist& twist)
{

    double timePeriod = 0.02; // 20ms time period per loop
    
    currPose.x = currPose.x + timePeriod * twist.u * cos(currPose.theta);
    currPose.y = currPose.y + timePeriod * twist.u * sin(currPose.theta);
    currPose.theta = currPose.theta + timePeriod * twist.omega;

#ifdef __NAV_DEBUG__
    TeleplotPrint("x", currPose.x);
    TeleplotPrint("y", currPose.y);
    TeleplotPrint("theta", currPose.theta * 180/M_PI);
#endif

}

/**
 * Sets a destination in the lab frame.
 */
void Robot::SetDestination(const Pose& dest)
{
    digitalWrite(13, HIGH);
    
    Serial.print("Setting dest to: ");
    Serial.print(dest.x);
    Serial.print(", ");
    Serial.print(dest.y);
    Serial.print('\n');

    destPose = dest;
    robotState = ROBOT_DRIVE_TO_POINT;
}

bool Robot::CheckReachedDestination(void)
{
    bool retVal = false;

    float distError = sqrt(sq(xError) + sq(yError));

    if (distError < 3)
    {
        retVal = true;
    }

    return retVal;
}

void Robot::DriveToPoint(void)
{
    if(robotState == ROBOT_DRIVE_TO_POINT)
    {
        xError = destPose.x - currPose.x;
        yError = destPose.y - currPose.y;
        thetaError = atan2(yError, xError) - currPose.theta;

        float xVel = xError;
        float yVel = yError;
        float thetaVel = thetaError;

#ifdef __NAV_DEBUG__
        TeleplotPrint("Theta Error", thetaError * 180/M_PI);
        TeleplotPrint("X Drive Velocity", xVel);
        TeleplotPrint("Y Drive Velocity", yVel);
        TeleplotPrint("Theta Drive Velocity", thetaVel * 180/M_PI);
        TeleplotPrint("Desired Angle", atan2(yError, xError) * 180/M_PI);
#endif

        float uVel = (xVel*cos(currPose.theta) + yVel*sin(currPose.theta)) * KP_U;
        float vVel = 0 * KP_V;
        float omegaVel = thetaVel * KP_OMEGA;
        
        chassis.SetTwist(Twist(uVel, vVel, omegaVel));
    }
}

void Robot::HandleDestination(void)
{
    digitalWrite(13, LOW);
    EnterIdleState();
}