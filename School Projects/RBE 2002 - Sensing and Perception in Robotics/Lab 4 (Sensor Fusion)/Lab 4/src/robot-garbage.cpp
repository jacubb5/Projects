/**
 * robot-garbage.cpp is where servo and load cell interfaces are
 */
#include <servo32u4.h>
#include <HX711.h>
#include <robot.h>

using namespace std;

Servo32U4Pin5 servo;
EventTimer liftTimer;

int sumCount = 0;
int32_t sum = 0;
uint8_t AVERAGE_COUNT = 50;

void Robot::attachServo(void)
{
    servo.attach();
}

void Robot::setServoPosition(int milliseconds)
{
    servo.setTargetPos(milliseconds);
}

void Robot::updateServo(void)
{
    servo.update();
}

void Robot::BeginWeighing(void)
{
    servo.setTargetPos(600);
    robotState = ROBOT_LIFTING;
}

bool Robot::CheckServoAtPosition(void)
{
    return servo.checkAtPosition();
}

void Robot::HandleServoAtPosition(void)
{
    if (robotState == ROBOT_LIFTING)
    {
        robotState = ROBOT_WEIGHING;
        sumCount = AVERAGE_COUNT;
    }
}

bool timeDelay = true;
bool timerExpired = false;
void Robot::HandleWeightReading(void)
{
    Serial.print(""); // This is needed for the program to work for some reason
    if (robotState == ROBOT_WEIGHING)
    {
        if (timeDelay)
        {
            liftTimer.start(500); // Servo speed needed to be increased in order to lift the box, so a delay timer is needed
            timeDelay = false;
        }

        if (liftTimer.CheckExpired()) timerExpired = true; // Ensures that the code continues to run after CheckExpired() returns true

        if (timerExpired) {
            if (sumCount > 0)
            {
                weight = (weight - 16016) / 730.8; // Converts load cell reading to weight in grams
                sum += weight;
                sumCount -= 1;
            }


            if (sumCount == 0)
            {
                weight = sum / AVERAGE_COUNT; // Gets the average recorded weight
                Serial.print("^^^ WEIGHT (");
                Serial.print(weight);
                Serial.println(')');

                sum = 0;

                chassis.BeginDeadReckoning(garbageDistance - 6); // Inverse of previous dead reckoning
                robotState = ROBOT_DEAD_RECKONING;

                timeDelay = true;
                timerExpired = false;
            }
        }
    }
}