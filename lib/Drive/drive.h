#ifndef DRIVE_H
#define DRIVE_H

class Drive {
    public:
        Drive(int lm, int rm);
        void left(int speed);
        void right(int speed);
        void forward(int speed);
        void backward(int speed);
        void stop();
        void release();
        void drive(int speed, int lDir, int rDir);
        void motorTest(int speed, int timeDelay);
        void turn(int speed, int dir);
        void backupAndTurn(int dir, int speed = 100, int turnDelay = 500);
        void motors(int lSpeed, int rSpeed, int lDir, int rDir);
        void veer(int speed, float turn, int leftOrRight);

    private:
        float calculateTurn(int lDist, int rDist);
        int calculateDir(int lDist, int rDist);

        Adafruit_MotorShield m_AFMS;
        Adafruit_DCMotor *m_motorLeft;
        Adafruit_DCMotor *m_motorRight;

}


#endif // DRIVE_H
