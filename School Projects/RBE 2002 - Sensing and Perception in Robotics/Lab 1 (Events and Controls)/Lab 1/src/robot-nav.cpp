/**
 * robot-nav.cpp is where you should put navigation routines: turning, lining, etc.
 */

#include "robot.h"

#define LINE_SPEED 10
//volatile int jgrid = 0;
//volatile int igrid = 0;

void Robot::BeginOutAndBack(int8_t destination)
{
    if(robotState == ROBOT_IDLE)
    {
        Serial.print("Set dest: ");
        Serial.println(destination);
        jTarget = destination;

        chassis.EnterLineFollowing(LINE_SPEED);
        if (currDirection != 1) { // Checks if the bot is facing the right direction and turns if not
            BeginTurn(1 - currDirection);
        }
        robotState = ROBOT_OUTBOUND;
    }
}

/**
 * Starts the process of driving to a set of coordinates. If the coordinates input from the remote
 * are not the same as the current coordinates, the bot will prioritize driving along the i
 * direction (if it is not already at the i target) and will turn to face the target, and will then
 * attempt the same for the j direction.
 */
void Robot::BeginToCoordinates(int8_t xCoord, int8_t yCoord)
{
    if (robotState == ROBOT_IDLE)
    {
        iTarget = xCoord;
        jTarget = yCoord;

        Serial.print("X dest: ");
        Serial.print(iTarget);
        Serial.print("   Y dest: ");
        Serial.println(jTarget);

        if (!(iGrid == iTarget && jGrid == jTarget)) // Does nothing if input coordinates match current coordinates
        {
            if (iGrid == iTarget) {
                if (jTarget > jGrid) {
                    targetDirection = 1;
                } else {
                    targetDirection = 3;
                }
            } else {
                if (iTarget > iGrid) {
                    targetDirection = 0;
                } else {
                    targetDirection = 2;
                }
            }

            BeginTurn((targetDirection - currDirection) % 4);
        }
    }
}

void Robot::HandleIntersection(void)
{
    Serial.print("X -- ");
    digitalWrite(13, !digitalRead(13)); // Toggles an LED at each detected intersection

    if(robotState != ROBOT_IDLE) 
    {
        // Changes iGrid or jGrid values depending on the heading of the bot
        if (currDirection==NORTH) { jGrid++; }
        else if (currDirection==SOUTH) { jGrid--; }
        else if (currDirection==EAST) { iGrid++; }
        else if (currDirection==WEST) { iGrid--; }
        
        if (!(iGrid == iTarget && jGrid == jTarget)) // Does nothing if current coordinates match target coordinates
        {
            if (iGrid == iTarget) {
                if (jTarget > jGrid) {
                    targetDirection = 1;
                } else {
                    targetDirection = 3;
                }
            } else {
                if (iTarget > iGrid) {
                    targetDirection = 0;
                } else {
                    targetDirection = 2;
                }
            }
        }

        // Center the bot in the intersection if at the target i or j location
        if ((currDirection==NORTH || currDirection==SOUTH) && jGrid==jTarget) {
            BeginDeadReckoning(8);
        } else if ((currDirection==EAST || currDirection==WEST) && iGrid==iTarget) {
            BeginDeadReckoning(8);
        }
    }

    /**
     * Check the state and act accordingly.
     */

    if(robotState == ROBOT_OUTBOUND)
    {
        // Very basic navigation
        jGrid++;

        if (jGrid == jTarget) {
            targetDirection = 3;
            BeginDeadReckoning(8);
        }
    }

    else if(robotState == ROBOT_INBOUND)
    {
        // Very basic navigation
        jGrid--;

        if (jGrid == 0) {
            targetDirection = 1;
            BeginDeadReckoning(8);
        }
    }

    Serial.print("Now at: ");
    Serial.print(iGrid);
    Serial.print(", ");
    Serial.println(jGrid);
}

void Robot::BeginTurn(int turns)
{
    chassis.BeginTurn(turns);
    robotState = ROBOT_TURNING;
}

void Robot::BeginDeadReckoning(int distance) {
    chassis.BeginDeadReckoning(distance);
    robotState = ROBOT_DEAD_RECKONING;
}

void Robot::HandleTurnComplete(void)
{
    /**
     * For arbitrary navigation, when the turn is complete, update the current direction
     * to match the target direction (which was set when commanding the turn).
     */
    currDirection = targetDirection;
    Serial.print("dir: ");
    Serial.println(currDirection);
    robotState = ROBOT_DRIVING;
    // if (currDirection == 1) robotState = ROBOT_OUTBOUND;
    // if (currDirection == 3) robotState = ROBOT_INBOUND;
    chassis.EnterLineFollowing(LINE_SPEED);

    if (robotState == ROBOT_INBOUND) {
        jTarget = 0;
        chassis.EnterLineFollowing(LINE_SPEED);
    }
    
    if (robotState == ROBOT_OUTBOUND) {
        EnterIdleState();
    }

}

/**
 * For arbitrary navigation, when the dead reckoning is complete, ensure
 * the robot continues its previous task.
 */ 
void Robot::HandleDeadReckoningComplete(void) {
    if (iGrid == iTarget && jGrid == jTarget) { // Enters idle if at the target coordinates
        EnterIdleState();
    } else { // Turns towards target coordinate (in i or j direction) if not at target
        int turns = (targetDirection - currDirection) % 4;
        BeginTurn(turns);
    }
}