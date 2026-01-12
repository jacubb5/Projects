#pragma once

#include <Arduino.h>
#include <Rangefinder.h>

/**
 * A class that uses input capture.
 */
class HC_SR04_IC : public Rangefinder 
{
protected:
    //for scheduling pings
    uint32_t lastPing = 0;
    uint32_t PING_INTERVAL = 125; //ms; change to your liking

    //define the states for the echo capture
    enum PULSE_STATE {PLS_IDLE, PLS_WAITING_LOW, PLS_WAITING_HIGH, PLS_CAPTURED};

    //and initialize to IDLE
    volatile PULSE_STATE pulseState = PLS_IDLE;

    uint8_t echoPin = 13;
    uint8_t trigPin = 0;

    // for keeping track of echo duration
    volatile uint32_t pulseStart = 0;
    volatile uint32_t pulseEnd = 0;

    // lastPingCheck tracks the last time we checked to send a ping to avoid interference
    uint32_t lastPingCheck = 0;

public:
    HC_SR04_IC(uint8_t trig) {trigPin = trig;}

    // must call init() to set up pins and interrupts
    // the isr should call an instance of ISR_echo
    void init(void);

    // command a ping -- need to call regularly
    void commandPing(void);

public:
    /** \brief Returns true upon receiving a new echo and false otherwise.
     * The last recorded distance will be stored in distance.
     * */
    bool getDistance(float& distance);

    // ISR for the echo pin
    void ISR_echo(void);
};

extern HC_SR04_IC hc_ic;