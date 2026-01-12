#include "LineSensor.h"

void LineSensor::Initialize(void)
{
    pinMode(leftSensorPin, INPUT);
    pinMode(rightSensorPin, INPUT);
}

int16_t LineSensor::CalcError(void) 
{ 
    /* TODO: Implement error calculation. */
    return 0;
}

/**
 * Reads the left sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadLeftSensor(void)
{
    // ADC of Left (White) = 455
    // ADC of Left (BlacK) = 577

    uint16_t leftSensor;
    double calculatedLeftSensor = (analogRead(A1) - 455.0) * 100.0/(577.0-455.0); // Linearizes output

    // Limits output from 0 to 100
    if (calculatedLeftSensor < 0) {
        leftSensor = 0;
    } else if (calculatedLeftSensor > 100) {
        leftSensor = 100;
    } else {
        leftSensor = calculatedLeftSensor;
    }

    return leftSensor;
}


/**
 * Reads the right sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadRightSensor(void)
{
    // ADC of Left (White) = 42
    // ADC of Left (BlacK) = 724
    
    uint16_t rightSensor;
    double calculatedRightSensor = (analogRead(A0) - 42.0) * 100.0/(724.0-42.0); // Linearizes output

    // Limits output from 0 to 100
    if (calculatedRightSensor < 0) {
        rightSensor = 0;
    } else if (calculatedRightSensor > 100) {
        rightSensor = 100;
    } else {
        rightSensor = calculatedRightSensor;
    }

    return rightSensor;
}

/**
 * Reads the leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor1(void) { // DO NOT USE, VERY SENSITIVE AND INACCURATE
    uint16_t black = 574;
    uint16_t white = 350;
    
    double sensor1 = (100.0/(black-white)) * (analogRead(A1)-white);

    if (sensor1 > 100) {
        sensor1 = 100;
    } else if (sensor1 < 0) {
        sensor1 = 0;
    }

    return sensor1;
}
/**
 * Reads the 2nd to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor2(void) {
    uint16_t black= 550;
    uint16_t white = 34;
    
    double sensor2 = (100.0/(black-white)) * (analogRead(A7)-white);
    
    if (sensor2 > 100) {
        sensor2 = 100;
    } else if (sensor2 < 0) {
        sensor2 = 0;
    }

    return sensor2;
}
/**
 * Reads the 3rd to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor3(void) {
    uint16_t black = 780;
    uint16_t white = 33;
    
    double sensor3 = (100.0/(black-white)) * (analogRead(A6)-white);
    
    if (sensor3 > 100) {
        sensor3 = 100;
    } else if (sensor3 < 0) {
        sensor3 = 0;
    }
    
    return sensor3;
}
/**
 * Reads the 4th to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor4(void) {
    uint16_t black = 790;
    uint16_t white = 33;

    double sensor4 = (100.0/(black-white)) * (analogRead(A0)-white);
    
    if (sensor4 > 100) {
        sensor4 = 100;
        
    } else if (sensor4 < 0) {
        sensor4 = 0;
    }

    return sensor4;
}
/**
 * Reads the 5th to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor5(void) {
    uint16_t black = 790;
    uint16_t white = 33;
    
    double sensor5 = (100.0/(black-white)) * (analogRead(A11)-white);
    
    if (sensor5 > 100) {
        sensor5 = 100;
    } else if (sensor5 < 0) {
        sensor5 = 0;
    }
    
    return sensor5;
}

/**
 * Calculates the weighted average of sensors 2 through 5 by multiplying each sensor reading
 * by its distance to the centerline of the Romi, and then dividing it by the sum of the distances
 * of all sensors to the center of the Romi.
 */
int16_t LineSensor::calcLineAverage(void) { // Gets the average of sensors 2 through 5
    double sensorDistance = 0.8; // cm
    double weightedSum = (ReadSensor2()*sensorDistance*-2) + (ReadSensor3()*sensorDistance*-1) +
                        (ReadSensor4()*sensorDistance*1) + (ReadSensor5()*sensorDistance*2);
    double distanceSum = sensorDistance * (6);

    double retVal = weightedSum / distanceSum;

    return retVal;
}


bool prevVal = false;
/**
 * Checks if both the left and right line sensors detect white simultaneously
 * and returns true if so. Will only return true once and will not return true
 * again until leaving the previous intersection and detecting another intersection.
 */
bool LineSensor::CheckIntersection(void)
{
    bool retVal = false;

    if (ReadLeftSensor() < 25 && ReadRightSensor() < 25) {
        if (!prevVal) {
            retVal = true;
        }
        prevVal = true;
    } else {
        prevVal = false;
    }
    
    return retVal;
}

bool leftVal = false;
bool rightVal = false;
bool prevLeftVal = false;
bool prevRightVal = false;
bool holdLeftVal = false;
bool holdRightVal = false;
/**
 * Will check if either line sensor detects a line and will return true
 * if so. Each line detector will only detect a line once.
 */
bool LineSensor::CheckOverLine(void)
{
    bool retVal = false;
    
    if (ReadLeftSensor() < 25 && prevLeftVal == false) { // Ensures the left value will only be registered once
        leftVal = true;
        holdLeftVal = true;
    } else {
        leftVal = false;
    }
    
    if (ReadRightSensor() < 25 && prevRightVal == false) { // Ensures the right value will only be registered once
        rightVal = true;
        holdRightVal = true;
    } else {
        rightVal = false;
    }

    if (holdLeftVal && holdRightVal) { // Checks if both the left and right sensor previously crossed the line, then returns true
        holdLeftVal = false;
        holdRightVal = false;
        retVal = true;
    }

    prevLeftVal = leftVal;
    prevRightVal = rightVal;
    
    return retVal;
}