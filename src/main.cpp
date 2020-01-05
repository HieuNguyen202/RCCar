#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Ultrasonic.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Ultrasonic lDist(2, 3);
Ultrasonic rDist(4, 5);

// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

void left(int speed);
void right(int speed);
void forward(int speed);
void backward(int speed);
void stop();
void release();
void drive(int speed, int lDir, int rDir);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hugh's RC car!");

  AFMS.begin();  // create with the default frequency 1.6KHz
}

void loop() {
    Serial.println(String(lDist.read()) + " <-> " + String(rDist.read()));
    delay(100);
}

void drive(int speed, int lDir, int rDir){
    if (speed < 0 || speed > 255)
        return;
    motor1->run(lDir);
    motor2->run(rDir);
    motor1->setSpeed(speed);
    motor2->setSpeed(speed);
}

void left(int speed){
    drive(speed, BACKWARD, FORWARD);
}

void right(int speed){
    drive(speed, FORWARD, BACKWARD);
}

void forward(int speed){
    drive(speed, FORWARD, FORWARD);
}

void backward(int speed){
    drive(speed, BACKWARD, BACKWARD);
}

void stop(){
    drive(0, BRAKE, BRAKE);
}

void release(){
    drive(0, RELEASE, RELEASE);
}