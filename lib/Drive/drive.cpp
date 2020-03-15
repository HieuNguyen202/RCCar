
#include <Adafruit_MotorShield.h>
#include "misc.h"
#include "drive.h"

#define TOO_CLOSE(dist) (dist >= 0 && dist < 7)     // in centimeters
#define CLOSE(dist) (dist >= 7 && dist < 20)         // in centimeters
#define FAR(dist) (dist >= 20)          // in centimeters
#define DIR_LEFT  0
#define DIR_RIGHT 1
#define INVALID_DISTANCE 357
#define NO_MOTOR_MODE 0

// void displayDistance(){
//     char buf[30];
//     sprintf(buf, "Distances: %03d %03d", lDist.read(), rDist.read());
//     Serial.println(buf);
// }

Drive::Drive(int lm, int rm) {
    if (lm < 1 || lm > 4 || rm < 1 || rm > 4){
        // Bad parameters, do nothing
    } else {
        m_AFMS = Adafruit_MotorShield(); 
        m_motorLeft  = m_AFMS.getMotor(lm);
        m_motorRight = m_AFMS.getMotor(rm);
    }
}

void Drive::motorTest(int speed, int timeDelay){
    forward(speed);     delay(timeDelay);
    backward(speed);    delay(timeDelay);
    left(speed);        delay(timeDelay);
    right(speed);       delay(timeDelay);
    release();
}

void Drive::backupAndTurn(int dir, int speed = 100 , int turnDelay = 500){
    backward(speed);    delay(200);
    turn(speed, dir);   delay(turnDelay);
    release();
}

/* Obstacle avoidance algorithms that calculate the best turn power
 * to avoid walls. */
float Drive::calculateTurn(int lDist, int rDist) {
    if (min(lDist, rDist) > 50)
        return 0;

    float distDiffFactor = mapfloat(abs(rDist - lDist), 0, 50, 0, 1);
    float urgentFactor = 1 - mapfloat(min(lDist, rDist), 0, 50, 0, 1);
    distDiffFactor = distDiffFactor > 1.0 ? 1.0 : distDiffFactor;
    distDiffFactor = distDiffFactor < 0 ? 0 : distDiffFactor;
    urgentFactor = urgentFactor > 1.0 ? 1.0 : urgentFactor;
    urgentFactor = urgentFactor < 0 ? 0 : urgentFactor;
    // char buf[30];
    // char distDiffFactorStr[6];
    // char urgentFactorStr[6];
    // char resultStr[6];
    // dtostrf(distDiffFactor, 5, 2, distDiffFactorStr);
    // dtostrf(urgentFactor, 5, 2, urgentFactorStr);
    // dtostrf(distDiffFactor * urgentFactor, 5, 2, resultStr);
    // sprintf(buf, "Factors: %s %s %s", distDiffFactorStr, urgentFactorStr,  resultStr);
    // Serial.println(buf);
    return 0.01 * distDiffFactor + 0.99 * urgentFactor;
}

/* Decide whether to turn left or right */
int Drive::calculateDir(int lDist, int rDist) {
    return rDist > lDist ? DIR_RIGHT : DIR_LEFT;
}

void Drive::turn(int speed, int leftOrRight) {
    if (leftOrRight == DIR_LEFT){
        left(speed);
    } else if (leftOrRight == DIR_RIGHT){
        right(speed);
    }
}

// turn 0 - 1
void Drive::veer(int speed, float turn, int leftOrRight){
    int lSpeed, rSpeed, mSpeed;
    if (speed < 0 || speed > 255)
        return;
    lSpeed = rSpeed = speed;
    mSpeed = int((1.0 - turn) * float(speed));
    if (leftOrRight == DIR_LEFT){
        lSpeed = mSpeed;
    } else if (leftOrRight == DIR_RIGHT){
        rSpeed = mSpeed;
    } else {
        return;
    }
    motors(lSpeed, rSpeed, FORWARD, FORWARD);
}

void Drive::motors(int lSpeed, int rSpeed, int lDir, int rDir){
    if (lSpeed < 0 || lSpeed > 255)
        return;
    if (rSpeed < 0 || rSpeed > 255)
        return;
#if NO_MOTOR_MODE
    char buf[30];
    if (lDir == BACKWARD) lSpeed = -lSpeed;
    if (rDir == BACKWARD) rSpeed = -rSpeed;
    sprintf(buf, "Motors: %03d %03d", lSpeed, rSpeed);
    Serial.println(buf);
#else
    motor1->run(lDir);
    motor2->run(rDir);
    motor1->setSpeed(lSpeed);
    motor2->setSpeed(rSpeed);
#endif
}

void Drive::drive(int speed, int lDir, int rDir){
    motors(speed, speed, lDir, rDir);
}

void Drive::left(int speed){
    drive(speed, BACKWARD, FORWARD);
}

void Drive::right(int speed){
    drive(speed, FORWARD, BACKWARD);
}

void Drive::forward(int speed){
    drive(speed, FORWARD, FORWARD);
}

void Drive::backward(int speed){
    drive(speed, BACKWARD, BACKWARD);
}

void Drive::stop(){
    drive(0, BRAKE, BRAKE);
}

void Drive::release(){
    drive(0, RELEASE, RELEASE);
}
