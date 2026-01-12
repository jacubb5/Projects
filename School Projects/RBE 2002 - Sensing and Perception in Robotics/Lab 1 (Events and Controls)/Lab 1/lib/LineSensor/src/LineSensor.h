#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * Set to whatever you are using.
 */
#define LEFT_LINE_SENSOR    A0
#define RIGHT_LINE_SENSOR   A4

class LineSensor
{
protected:
    uint8_t leftSensorPin = LEFT_LINE_SENSOR;
    uint8_t rightSensorPin = RIGHT_LINE_SENSOR;

public:
    LineSensor(void) {}
    uint16_t ReadLeftSensor(void);
    uint16_t ReadRightSensor(void);

    uint16_t ReadSensor1(void);
    uint16_t ReadSensor2(void);
    uint16_t ReadSensor3(void);
    uint16_t ReadSensor4(void);
    uint16_t ReadSensor5(void);

    int16_t calcLineAverage(void);

    void Initialize(void);
    
    int16_t CalcError(void);
    bool CheckIntersection(void);
    bool CheckOverLine(void);
};