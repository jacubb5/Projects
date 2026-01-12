#include "chassis.h"
#include "Romi32U4MotorTemplate.h"

Romi32U4EncodedMotor<LEFT_XOR, LEFT_B, PWM_L, DIR_L, OCR_L> leftMotor("L");
Romi32U4EncodedMotor<RIGHT_XOR, RIGHT_B, PWM_R, DIR_R, OCR_R> rightMotor("R");

void Chassis::SetMotorKp(float kp) {leftMotor.Kp = kp; rightMotor.Kp = kp;}
void Chassis::SetMotorKi(float ki) {leftMotor.Ki = ki; rightMotor.Ki = ki; leftMotor.sumError = 0; rightMotor.sumError = 0;}
void Chassis::SetMotorKd(float kd) {leftMotor.Kd = kd; rightMotor.Kd = kd;}

/**
 * Because it's declared static, we initialize Chassis::loopFlag here.
 */
uint8_t Chassis::loopFlag = 0;

/**
 * For taking snapshots and raising the flag.
 */
void Chassis::Timer4OverflowISRHandler(void) 
{
    loopFlag++;

    leftMotor.speed = leftMotor.CalcEncoderDelta();
    rightMotor.speed = rightMotor.CalcEncoderDelta();
}

/**
 * ISR for timing. On Timer4 overflow, we take a 'snapshot' of the encoder counts 
 * and raise a flag to let the program it is time to execute the PID calculations.
 */
ISR(TIMER4_OVF_vect)
{
   Chassis::Timer4OverflowISRHandler();
}

/**
 * Sets up a hardware timer on Timer4 to manage motor control on a precise schedule.
 * 
 * We set the timer to set an interrupt flag on overflow, which is handled
 * by ISR(TIMER4_OVF_vect) below.
 */
void Chassis::InitializeMotorControlTimer(void)
{
    Serial.println("InitTimer");
    // Disable interupts while we mess with the Timer4 registers
    cli(); 
  
    // Set up Timer4
    TCCR4A = 0x00; // Disable output to pins
    TCCR4B = 0x0A; // Sets the prescaler -- see pp. 167-8 in datasheet
    TCCR4C = 0x00; // Disables output to pins (but see below for buzzer)
    TCCR4D = 0x00; // Normal mode: count up and roll-over

    /**
     * Calculate TOP based on prescaler and loop duration. Note that loop is in integer ms --
     * there may be some rounding. Multiples of 4 ms will be exact.
     */
    uint16_t top = ((CONTROL_LOOP_PERIOD_MS * 16000ul) >> 9) - 1; // divides by 512

    /**
     * Here we do a little trick to allow full 10-bit register access. 
     * We have 2 _bits_ in TC4H that we can use to add capacity to TOP.
     * 
     * Note that the maximum period is limited by TOP = 0x3FF. If you want
     * a longer period, you'll need to adjust the pre-scaler.
     * 
     * There is no minumum period, but precision breaks down with low values, 
     * unless you adjust the pre-scaler, but the encoder resolution is limited,
     * so you only want to go so fast.
    */
    uint8_t highbits = top / 256;
    uint8_t lowbits = top - highbits;
    TC4H = highbits; OCR4C = lowbits;

    // Enable overflow interrupt
    TIMSK4 = 0x04; 

    /**
     * Uncommenting the following lines will pipe the timer signal to pin 6, 
     * which controls the buzzer. The pin will toggle at the loop rate, which
     * allows you to check that the timing is correct. It will also make a lot
     * of noise, so do so sparingly.
     */
    // TCCR4C = 0x04;
    // pinMode(6, OUTPUT);

    // Re-enable interrupts
    sei(); 

    Serial.println("/InitTimer");
}

/**
 * The main Chassis loop.
 */
bool Chassis::ChassisLoop(void)
{
    bool retVal = false;

    if(loopFlag)
    {
        if(loopFlag > 1) Serial.println("Missed an update in Robot::RobotLoop()!");

#ifdef __LOOP_DEBUG__
        Serial.print(millis());
        Serial.print('\n');
#endif

        // add synchronous, pre motor update actions here
        LineFollowingUpdate();
        HeadingUpdate();

        // motor updates
        UpdateMotors();

        // add synchronous, post motor update actions here

        loopFlag = 0;

        retVal = true;
    }

    return retVal;
}

/**
 * Some motor methods.
 */
void Chassis::InitializeMotors(void)
{
    Romi32U4MotorBase::InitializePWMTimerAndInterrupts();

    leftMotor.InitializeMotor();
    rightMotor.InitializeMotor();
}

void Chassis::SetMotorEfforts(int16_t left, int16_t right) 
{
    leftMotor.SetMotorEffortDirect(left); 
    rightMotor.SetMotorEffortDirect(right);
}

void Chassis::UpdateMotors(void)
{
    leftMotor.ControlMotorSpeed();
    rightMotor.ControlMotorSpeed();
}

/**
 * SetWheelSpeeds converts the linear wheel speeds (axes relative to ground) to motor speeds.
 */
void Chassis::SetWheelSpeeds(float leftSpeedCMperSec, float rightSpeedCMperSec)
{
    /** 
     * WARNING: THIS DOES NOT WORK AS IS!!!
     * 
     * TODO: Add code to convert cm/sec -> encoder ticks/control interval.
     * 
     * 0.01527cm per encoder tick -> 65.5 encoder ticks per cm
     * 50 control intervals per second -> 0.02 seconds per control interval
     * 1cm/s = 65.5 * 0.02 = 1.310 ticks per control inerval
     * 
     * WARNING: USING RACING CODE HAS AN INTERVAL 5 TIMES FASTER
     */
    float leftTicksperInterval = (leftSpeedCMperSec*65.5) / 50.0 / (20.0 / CONTROL_LOOP_PERIOD_MS);
    float rightTicksperInterval = (rightSpeedCMperSec*65.5) / 50.0 / (20.0 / CONTROL_LOOP_PERIOD_MS);
    leftMotor.SetTargetSpeed(leftTicksperInterval);
    rightMotor.SetTargetSpeed(rightTicksperInterval);
}

/**
 * For driving forward.
 * 
 * fwdSpeed in cm/sec; wheel speed in cs/sec
 */
void Chassis::DriveAt(float fwdSpeed)
{
    /** This one is ready to go. */
    SetWheelSpeeds(fwdSpeed, fwdSpeed);

    chassisState = CHASSIS_DEAD_RECKONING;
}

/**
 * For commanding a turn turning.
 * 
 * angVel in dps -> wheelSpeed in cm/sec
 */
void Chassis::TurnAt(float angVel)
{
    /** 
     * TODO: Call SetWheelSpeeds with the correct values for turning. A positive value for angVel
     * must lead to a positive turn direction.
     * 
     * Wheel to wheel = 7cm
     * Circumference of drive base = 7pi
     * Wheel distance per degree = circumference / 360 = 7pi/360
     */
    float CMtoDegperSec = 7.0 * M_PI * 2.0 / 360.0;
    
    SetWheelSpeeds(-angVel*CMtoDegperSec, angVel*CMtoDegperSec);

    chassisState = CHASSIS_TURNING;
}

/**
 * Functions related to line following and intersection detection.
 */
void Chassis::EnterLineFollowing(float speed) 
{
    Serial.println(" -> LINING"); 
    lineSpeed = speed;
    chassisState = CHASSIS_LINING;
}

int16_t prevError = 0;
/**
 * For line following.
 */
void Chassis::LineFollowingUpdate(void)
{
    /* Only update speeds if in line following mode. */
    if(chassisState == CHASSIS_LINING)
    {
        double KP_LINE = 0.1;
        double KD_LINE = 0.2;
       
        int16_t error = -lineSensor.calcLineAverage();

        float turnEffort = error*KP_LINE + (error - prevError)*KD_LINE;
    
        SetWheelSpeeds(lineSpeed + turnEffort, lineSpeed - turnEffort);

        prevError = error;
    }
}

/**
 * Turning.
 */
int turnsRemaining = 0;
void Chassis::BeginTurn(uint8_t turns)
{
    int8_t intTurns = turns;
    Serial.print("--> TURN (");
    Serial.print(intTurns);
    Serial.println(')');

    // Determine whether to turn CW or CCW to efficiently turn to a direction
    if (turns % 4 == 0) {
        chassisState = CHASSIS_TURNING;
        return;
    } else if ((turns % 4) <= 2) {
        TurnAt(60);
        turnsRemaining = abs(turns % 4);
    } else if ((turns % 4) > 2) {
        TurnAt(-60);
        turnsRemaining = abs(turns % 4 - 2);
    }

    // Multiply line detection by 2 (left and right sensor each detect once per line)
    turnsRemaining *= 2;

    Serial.print("INITIAL REMAINING (");
    Serial.print(turnsRemaining);
    Serial.println(')');

    chassisState = CHASSIS_TURNING;
}

/**
 * Drive a specified distance at 5cm/sec using a timer to detect when to stop
 */
void Chassis::BeginDeadReckoning(int distance)
{
    Serial.print("--> DRIVING (");
    Serial.print(distance);
    Serial.println(')');

    if (distance > 0) {
        DriveAt(5);
    } else if (distance < 0) {
        DriveAt(-5);
    }
    deadReckoningTimer.start(abs(distance) * 200);

    chassisState = CHASSIS_DEAD_RECKONING;
}

/**
 * Counts down the turns remaining as lines are detected and returns true of no turns remain
 */
bool Chassis::CheckTurnComplete(void)
{
    if (CheckOverLine() && chassisState == CHASSIS_TURNING) {
        turnsRemaining--;
        Serial.print("--> REMAINING (");
        Serial.print(turnsRemaining);
        Serial.println(')');
    }

    if (turnsRemaining == 0 && chassisState == CHASSIS_TURNING) {
        return true;
    }
    
    return false;
}

/**
 * A simple checker just checks if the timer has expired. Note that the timer will only return
 * true once. Then it stops running.
 */
bool Chassis::CheckDeadReckoningComplete(void)
{
    return deadReckoningTimer.CheckExpired();
}

float prevGyroEst = 0;
float gyroBias = 0;
float prevGyroBias = 0;
float kappa = 0.1;
float epsilon = 0.05;

float prevGyroHeadingEst = 0;
float gyroHeadingBias = 0;
float prevGyroHeadingBias = 0;
float kappaHeading = 0.1;
float epsilonHeading = 0.05;
/**
 * Checks the IMU and uses a complementary filter with the accelerometer and
 * gyroscope to estimate the current pitch angle and heading on a ramp
 */
bool Chassis::CheckIMU(float& currentAngleEst, float& currentHeadingEst)
{
    bool retVal = false;
    
    if(imu.checkForNewData())
    {
        // Calculate the pitch angle
        float accelTheta = atan2(-imu.a.x,imu.a.z) * 180 / M_PI; // Calculates angle in degrees
        int16_t gyro = imu.g.y;
        
        // Theta_k = theta_k-1 + deltaT * s * gamma
        // ODR at 104Hz, deltaT = 1/ODR = 9.62 millisecond periods
        // s = sensitivity = 17.50 mdps/LSB
        // gamma = reading
        
        float gyroEst = prevGyroEst + (gyro-gyroBias) * (17.50 / 1000) * (1.0/104);
        prevGyroEst = gyroEst;
        
        gyroBias = prevGyroBias - epsilon * (1000/17.50) * (104/1.0) * (accelTheta-gyroEst);

        float filteredAngle = (1-kappa)*gyroEst + kappa*accelTheta;

        currentAngleEst = filteredAngle;


        // Calculate the heading angle on the ramp
        float accelYReading = atan2(-imu.a.y, imu.a.x) * 180 / M_PI;
        int16_t gyroZReading = imu.g.z;

        float gyroHeadingEst = prevGyroHeadingEst + (gyroZReading-gyroHeadingBias) * (17.50 / 1000) * (1.0/104);
        prevGyroHeadingEst = gyroHeadingEst;
        
        gyroHeadingBias = prevGyroHeadingBias - epsilonHeading * (1000/17.50) * (104/1.0) * (accelYReading-gyroHeadingEst);

        float filteredHeadingAngle = (1-kappaHeading)*gyroHeadingEst + kappaHeading*accelYReading;

        currentHeadingEst = filteredHeadingAngle;
        heading = filteredHeadingAngle;


        retVal = true;
        
        #ifdef __IMU_DEBUG__
            TeleplotPrint("a.x", imu.a.x);
            TeleplotPrint("a.y", imu.a.y);
            TeleplotPrint("a.z", imu.a.z);
            TeleplotPrint("g.x", imu.g.x);
            TeleplotPrint("g.y", imu.g.y);
            TeleplotPrint("g.z", imu.g.z);
            TeleplotPrint("Accelerometer Theta", accelTheta);
            TeleplotPrint("Gyro ADC Reading", gyro);
            TeleplotPrint("Gyro Predicted Angle", gyroEst);
            TeleplotPrint("Filtered Angle", filteredAngle);
            TeleplotPrint("Filtered Heading Angle", filteredHeadingAngle);
        #endif
    }

    return retVal;
}

void Chassis::TeleplotPrint(const char* var, float value)
{
    Serial.print('>');
    Serial.print(var);
    Serial.print(':');
    Serial.print(value);
    Serial.print('\n');
}

/**
 * For following a heading
 */
void Chassis::BeginHeadingDrive(float speed)
{
    Serial.println("--> FOLLOWING HEADING");
    headingDriveSpeed = speed;
    chassisState = CHASSIS_HEADING_DRIVE;
}

int16_t prevHeadingError = 0;
/**
 * For heading following.
 */
void Chassis::HeadingUpdate(void)
{
    /* Only update speeds if in line following mode. */
    if(chassisState == CHASSIS_HEADING_DRIVE)
    {
        double KP_HEADING = 0.4;
       
        int16_t error = heading;

        float turnEffort = error*KP_HEADING;
    
        SetWheelSpeeds(headingDriveSpeed + turnEffort, headingDriveSpeed - turnEffort);

        prevHeadingError = error;
    }
}