/**
 * robot-remote.cpp implements features of class Robot that are related to processing
 * remote control commands. It also manages modes. You might want to trim away some of 
 * the code that isn't needed later in the term.
 */
#include "robot.h"

#include <ir_codes.h>
#include <IRdecoder.h>

// Values of target coordinates for traversing the grid
int8_t xTarget = 0;
int8_t yTarget = 0;

/**
 * IRDecoder decoder is declared extern in IRdecoder.h (for ISR purposes), 
 * so we _must_ name it decoder.
 */
#define IR_PIN 17
IRDecoder decoder(IR_PIN);

void Robot::HandleKeyCode(int16_t keyCode)
{ 
    Serial.println(keyCode);

    // Regardless of current state, if ENTER is pressed, go to idle state
    if(keyCode == ENTER_SAVE) { EnterIdleState(); keyString = "";}


    // If STOP/MODE is pressed, it toggles control mode (auto <-> teleop)
    else if(keyCode == STOP_MODE) 
    {
        if(robotCtrlMode == CTRL_AUTO) {EnterTeleopMode(); }
        else if(robotCtrlMode == CTRL_TELEOP) {EnterAutoMode(); }
        EnterIdleState(); // Idle to avoid surprises
        keyString = "";
    }

    if(robotCtrlMode == CTRL_AUTO)
    {
        switch(keyCode)
        {
            case REWIND:
                BeginOutAndBack(keyString.toInt());
                keyString = "";
                break;
            case UP_ARROW:
                chassis.EnterLineFollowing(keyString.toInt());
                keyString = "";
                break;
            case VOLminus:
                xTarget = keyString.toInt();
                keyString = "";
                break;
            case PLAY_PAUSE:
                yTarget = keyString.toInt();
                keyString = "";
                break;
            case SETUP_BTN:
                BeginToCoordinates(xTarget, yTarget);
                break;
            case VOLplus:
                BeginWeighing();
                break;
            case RIGHT_ARROW:
                BeginBangBang();
                break;
            case NUM_1:
            case NUM_2:
            case NUM_3:
                keyString += (char)(keyCode + 33);
                break;
            case NUM_4:
            case NUM_5:
            case NUM_6:
                keyString += (char)(keyCode + 32);
                break;
            case NUM_7:
            case NUM_8:
            case NUM_9:
                keyString += (char)(keyCode + 31);
                break;
            case NUM_0_10:
                keyString += '0';
                break;
        }
    }

    else if(robotCtrlMode == CTRL_TELEOP)
    {
        switch(keyCode)
        {
            case UP_ARROW:
                chassis.DriveAt(20);
                break;
            case RIGHT_ARROW:
                chassis.TurnAt(-90);
                break;
            case DOWN_ARROW:
                chassis.DriveAt(-20);
                break;
            case LEFT_ARROW:
                chassis.TurnAt(90);
                break;
            case ENTER_SAVE:
                chassis.DriveAt(0);
                break;
            case VOLminus:
                setServoPosition(1600);
                break;
            case VOLplus:
                setServoPosition(1000);
                break;
            case PLAY_PAUSE:
                BeginUltrasonic(5);
        }
    }    
}

void Robot::EnterTeleopMode(void)
{
    Serial.println("-> TELEOP");
    robotCtrlMode = CTRL_TELEOP;
}

void Robot::EnterAutoMode(void)
{
    Serial.println("-> AUTO");
    robotCtrlMode = CTRL_AUTO;
}
