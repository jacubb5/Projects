#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * Set to whatever you are using.
 */
#define LINE_SENSOR_1       A6
#define LINE_SENSOR_2       A2
#define LINE_SENSOR_3       A3
#define LINE_SENSOR_4       A0
#define LINE_SENSOR_5       A4
#define LINE_SENSOR_6       A11

class LineSensor
{
protected:
    uint8_t sensor1Pin = LINE_SENSOR_1;
    uint8_t sensor2Pin = LINE_SENSOR_2;
    uint8_t sensor3Pin = LINE_SENSOR_3;
    uint8_t sensor4Pin = LINE_SENSOR_4;
    uint8_t sensor5Pin = LINE_SENSOR_5;
    uint8_t sensor6Pin = LINE_SENSOR_6;

public:
    LineSensor(void) {}
    uint16_t ReadLeftSensor(void);
    uint16_t ReadRightSensor(void);

    uint16_t ReadSensor1(void);
    uint16_t ReadSensor2(void);
    uint16_t ReadSensor3(void);
    uint16_t ReadSensor4(void);
    uint16_t ReadSensor5(void);
    uint16_t ReadSensor6(void);

    int16_t calcLineAverage(void);

    void Initialize(void);
    
    int16_t CalcError(void);
    bool CheckIntersection(void);
    bool CheckOverLine(void);
};