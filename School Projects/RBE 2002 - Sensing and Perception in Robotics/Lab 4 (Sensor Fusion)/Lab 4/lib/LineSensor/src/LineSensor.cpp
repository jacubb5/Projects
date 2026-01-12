#include "LineSensor.h"

bool DEBUG_MODE = false;

void LineSensor::Initialize(void)
{
    pinMode(sensor1Pin, INPUT);
    pinMode(sensor2Pin, INPUT);
    pinMode(sensor3Pin, INPUT);
    pinMode(sensor4Pin, INPUT);
    pinMode(sensor5Pin, INPUT);
    pinMode(sensor6Pin, INPUT);

}

/**
 * Reads the leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor1(void) { // DO NOT USE, VERY SENSITIVE AND INACCURATE
    int16_t black = 940;
    int16_t white = 650;
    
    double sensor1 = (100.0/(black-white)) * (analogRead(A6)-white);

    if (sensor1 > 100) {
        sensor1 = 100;
    } else if (sensor1 < 0) {
        sensor1 = 0;
    }

    if (DEBUG_MODE) return analogRead(A6);
    return sensor1;
}
/**
 * Reads the 2nd to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor2(void) {
    int16_t black = 927;
    int16_t white = 520;
    
    double sensor2 = (100.0/(black-white)) * (analogRead(A2)-white);
    
    if (sensor2 > 100) {
        sensor2 = 100;
    } else if (sensor2 < 0) {
        sensor2 = 0;
    }

    if (DEBUG_MODE) return analogRead(A2);
    return sensor2;
}
/**
 * Reads the 3rd to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor3(void) {
    int16_t black = 912;
    int16_t white = 358;
    
    double sensor3 = (100.0/(black-white)) * (analogRead(A3)-white);
    
    if (sensor3 > 100) {
        sensor3 = 100;
    } else if (sensor3 < 0) {
        sensor3 = 0;
    }

    if (DEBUG_MODE) return analogRead(A3);
    return sensor3;
}
/**
 * Reads the 4th to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor4(void) {
    int16_t black = 912;
    int16_t white = 286;

    double sensor4 = (100.0/(black-white)) * (analogRead(A0)-white);
    
    if (sensor4 > 100) {
        sensor4 = 100;
        
    } else if (sensor4 < 0) {
        sensor4 = 0;
    }

    if (DEBUG_MODE) return analogRead(A0);
    return sensor4;
}
/**
 * Reads the 5th to leftmost sensor sensor and returns a value from 0 (white) to 100 (black)
 */
uint16_t LineSensor::ReadSensor5(void) {
    int16_t black = 919;
    int16_t white = 408;
    
    double sensor5 = (100.0/(black-white)) * (analogRead(A4)-white);
    
    if (sensor5 > 100) {
        sensor5 = 100;
    } else if (sensor5 < 0) {
        sensor5 = 0;
    }
    
    if (DEBUG_MODE) return analogRead(A4);
    return sensor5;
}
uint16_t LineSensor::ReadSensor6(void){
    int16_t black = 630;
    int16_t white = 370;

    double sensor6 = (100.0/(black-white))*(analogRead(A11)-white);

    if (sensor6 > 100) {
        sensor6 = 100;
    } else if (sensor6 < 0) {
        sensor6 = 0;
    }

    if (DEBUG_MODE) return analogRead(A11);
    return sensor6;

}

/**
 * Calculates the weighted average of sensors 2 through 5 by multiplying each sensor reading
 * by its distance to the centerline of the Romi, and then dividing it by the sum of the distances
 * of all sensors to the center of the Romi.
 */
int16_t LineSensor::calcLineAverage(void) { // Gets the average of sensors 2 through 5

    if (DEBUG_MODE)
    {
        Serial.print("Line 1: ");
        Serial.print(ReadSensor1());
        Serial.print("  Line 2: ");
        Serial.print(ReadSensor2());
        Serial.print("  Line 3: ");
        Serial.print(ReadSensor3());
        Serial.print("  Line 4: ");
        Serial.print(ReadSensor4());
        Serial.print("  Line 5: ");
        Serial.print(ReadSensor5());
        Serial.print("  Line 6: ");
        Serial.println(ReadSensor6());
    }

    double sensorDistance = 0.8; // cm
    double weightedSum = (ReadSensor1()*sensorDistance*-0.5) + (ReadSensor2()*sensorDistance*-1.5) + (ReadSensor3()*sensorDistance*-2.5) +
                        (ReadSensor4()*sensorDistance*0.5) + (ReadSensor5()*sensorDistance*1.5) + (ReadSensor6()*sensorDistance*2.5);
    double distanceSum = sensorDistance * 2*(0.5+1.5+2.5);

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

    uint8_t leftSensor = ReadSensor2();
    uint8_t rightSensor = ReadSensor5();

    // Serial.print("Left: ");
    // Serial.print(leftSensor);
    // Serial.print("  Right: ");
    // Serial.println(rightSensor);

    Serial.print(""); // Needs this to run properly


    if (leftSensor < 25 && rightSensor < 25)
    {
        if (!prevVal)
        {
            retVal = true;
        }
        prevVal = true;
    }
    else if (leftSensor > 75 && rightSensor > 75)
    {
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
    
    Serial.print("");

    uint8_t leftSensor = ReadSensor2();
    uint8_t rightSensor = ReadSensor5();

    // Serial.print("Left: ");
    // Serial.print(leftSensor);
    // Serial.print("  Right: ");
    // Serial.println(rightSensor);

    if (leftSensor < 60 && prevLeftVal == false) { // Ensures the left value will only be registered once
        leftVal = true;
        holdLeftVal = true;
    } else {
        leftVal = false;
    }
    
    if (rightSensor < 60 && prevRightVal == false) { // Ensures the right value will only be registered once
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