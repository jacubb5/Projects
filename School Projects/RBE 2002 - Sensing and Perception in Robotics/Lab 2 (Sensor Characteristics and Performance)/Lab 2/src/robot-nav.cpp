/**
 * robot-nav.cpp is where you should put navigation routines: turning, lining, etc.
 */

#include "robot.h"

using namespace std;

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
                    targetDirection = NORTH;
                } else {
                    targetDirection = SOUTH;
                }
            } else {
                if (iTarget > iGrid) {
                    targetDirection = EAST;
                } else {
                    targetDirection = WEST;
                }
            }

            BeginTurn((targetDirection - currDirection) % 4);
        }
    }
}

int remainingReadings = 0;
void Robot::BeginUltrasonic(int readings)
{
    robotState = SENSING_TEST;
    remainingReadings = readings;
}

void Robot::BeginBangBang(void) {
    robotState = ROBOT_FWD;
}

double prevAverage = 0;
double runningAverage = 0;
double zeta = 0.8;
double window[5] = {0};
int currReading = 0;
double output;
/**
 * When a distance reading is input, this function will process the data
 * to filter it into something more reliable and accurate. This will also
 * handle robot actions depending on the current state.
 */
void Robot::HandleDistanceReading(void)
{
    window[currReading] = distance;
    currReading++;
    currReading %= 5;
    
    if (distanceReading == PRINT_READINGS) // Sets output to raw distance inputs
    {
        output = distance;

        if (remainingReadings > 0) {
            Serial.print("Reading: ");
            Serial.println(output);
            remainingReadings--;
        }
    }
    else if (distanceReading == RUNNING_AVERAGE) // Sets output to the running average of distance inputs
    {
        // Calculates the running average of distance readings
        runningAverage = distance*(1-zeta) + zeta*prevAverage;
        prevAverage = runningAverage;
        output = runningAverage;

        if (remainingReadings > 0) {
            Serial.print("Running Average: ");
            Serial.println(output);
            remainingReadings--;
        }
    }
    else if (distanceReading == WINDOWED_AVERAGE) // Sets output to the windowed average of 5 distance inputs
    {
        double windowedAverage;
        double windowSum = 0;
        window[currReading] = distance;

        // Gets the average of window values
        for (int i = 0; i < 5; i++) {
            windowSum += distance;
        }
        windowedAverage = windowSum / 5;
        output = windowedAverage;
        
        if (remainingReadings > 0) {
            Serial.print("Windowed Average: ");
            Serial.println(output);
            remainingReadings--;
        }
    }
    else if (distanceReading == MEDIAN_FILTER) // Sets output to the windowed median of 5 distance inputs
    {
        double sortedWindow[5] = {window[0], window[1], window[2], window[3],  window[4]};

        // Sorts the window values
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (sortedWindow[i] > sortedWindow[j])
                {
                    double tempVal = sortedWindow[i];
                    sortedWindow[i] = sortedWindow[j];
                    sortedWindow[j] = tempVal;
                }
            }
        }
        double median = sortedWindow[2];
        output = median;

        if (remainingReadings > 0) {
            Serial.print("Median Filter: ");
            Serial.println(output);
            remainingReadings--;
        }
    }



    if (robotState == ROBOT_FWD || robotState == ROBOT_REV)
    {
        if(robotState == ROBOT_REV)
        {
            chassis.DriveAt(-20);

            if(output > 40) // guard condition
            {
                robotState = ROBOT_FWD;
            }
        }   
        else
        {
            chassis.DriveAt(5);

            if(output <= 25) // guard condition
            {
                robotState = ROBOT_REV;
            }
        } 
    }

    if (robotState == ROBOT_SEARCHING)
    {
        if (output < 20)
        {
            robotState = ROBOT_APPROACHING;
            foundTrash = 1;
        }
        else
        {
            targetDirection = (targetDirection + 1) % 4;
            BeginTurn((targetDirection - currDirection) % 4);
        }
    }

    if (robotState == ROBOT_APPROACHING)
    {
        if (output < 5) {
            EnterIdleState();
        }
    }


    #ifdef __ULTRASONIC_RESPONSE__
            Serial.print('>');
            Serial.print("Filtered Distance Output:");
            Serial.println(output);
            Serial.print('>');
            Serial.print("Raw Distance Reading:");
            Serial.println(distance);
    #endif
}

void Robot::HandleIntersection(void)
{
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

        Serial.print("X -- ");
        digitalWrite(13, !digitalRead(13)); // Toggles an LED at each detected intersection

        Serial.print("Now at: ");
        Serial.print(iGrid);
        Serial.print(", ");
        Serial.println(jGrid);
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
    if (currDirection != targetDirection) {
        Serial.print("dir: ");
        Serial.println(targetDirection);
    }
    currDirection = targetDirection;
    
    if (iGrid == iTarget && jGrid == jTarget && foundTrash == 0) {
        robotState = ROBOT_SEARCHING;
    } else if (!foundTrash) {
        robotState = ROBOT_DRIVING;
        chassis.EnterLineFollowing(LINE_SPEED);
    } else {
        chassis.EnterLineFollowing(LINE_SPEED);
    }
    // if (currDirection == 1) robotState = ROBOT_OUTBOUND;
    // if (currDirection == 3) robotState = ROBOT_INBOUND;
    
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
        //EnterIdleState();
        robotState = ROBOT_SEARCHING;
    } else { // Turns towards target coordinate (in i or j direction) if not at target
        int turns = (targetDirection - currDirection) % 4;
        BeginTurn(turns);
    }
}

