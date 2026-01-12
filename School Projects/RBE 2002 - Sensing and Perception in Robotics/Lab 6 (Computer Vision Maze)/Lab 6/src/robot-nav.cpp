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

bool Robot::CheckAprilTag(void)
{
    return camera.getTagCount();
}

bool Robot::CheckTagTimerExpired(void)
{
    return tagTimer.CheckExpired();
}

bool Robot::CheckApproachComplete(void)
{
    if (robotState == ROBOT_APPROACHING)
    {
        float headingTolerance = 5;
        float distanceTolerance = 5;
        
        if (abs(distError) < distanceTolerance && abs(turnError) < headingTolerance)
        {
            return true;
        }
    }

    return false;
}

void Robot::DriveToPoint(void)
{
    if(robotState == ROBOT_DRIVE_TO_POINT)
    {
        xError = destPose.x - currPose.x;
        yError = destPose.y - currPose.y;
        thetaError = atan2(yError, xError) - currPose.theta;

        while (thetaError > (M_PI)) {
            thetaError -= M_PI;
        }
        while (thetaError < -(M_PI)) {
            thetaError += M_PI;
        }

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

/**
 * Uses proportional control to align with an Apriltag
 */
void Robot::AlignWithTag(void)
{
    float targetDistance = 70; // Pixels, target tag width
    float targetTurn = 80; // Pixels, center of camera

    distError = targetDistance - tag.w;
    turnError = tag.cx - targetTurn;

    float distEffort = distError * KP_DIST;
    float turnEffort = turnError * KP_THETA;

    chassis.SetTwist(Twist(distEffort, 0, turnEffort));
}

void Robot::HandleDestination(void)
{
    digitalWrite(13, LOW);
    EnterSearchingState(true);
}

void Robot::HandleAprilTag(void)
{
    camera.readTag(tag);
    tagTimer.start(tagTimeout);
    
    if (robotState == ROBOT_SEARCHING || robotState == ROBOT_DRIVE_TO_POINT) EnterApproachingState();

#ifdef __CAMERA_DEBUG__
    Serial.print("Tag: ");
    Serial.print(tag.id); Serial.print('\t');
    Serial.print(tag.cx); Serial.print('\t');
    Serial.print(tag.cy); Serial.print('\t');
    Serial.print(tag.h); Serial.print('\t');
    Serial.print(tag.w); Serial.print('\t');
    Serial.print(tag.rot); //rotated angle; try turning the tag
    Serial.print('\n');
#endif

    // TODO: Handle the tag and process data in ROBOT_APPROACHING state
}

void Robot::HandleTagTimerExpired(void)
{
    if (robotState == ROBOT_APPROACHING)
    {
        Serial.println("??? LOST APRILTAG");
        
        EnterSearchingState(true);
    }
}

void Robot::HandleApproachComplete(void)
{
    uint16_t prevTagID = tag.id;
    Serial.print("TAG: ");
    Serial.println(prevTagID);

    if (prevTagID == 0)
    {
        EnterIdleState();
    }
    else
    {
        EnterTurningState(prevTagID % 2);
    }
}

void Robot::HandleTurnTimerExpired(void)
{
    EnterSearchingState(true);
}