#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

class BaseMotor {
    // propeties
    int M1; // center
    int M2; // left
    int M3; // right;
    
    float P = 0;
    float I = 0;
    float D = 0;
    float Z_SUM = 0;
    // methods
public:
    BaseMotor();
    BaseMotor(int, int, int);
    
    // setters
    void setupM1 (int);
    void setupM2 (int);
    void setupM3 (int);
   
    void setM1 (int);
    void setM2 (int);
    void setM3 (int);
    // getters
    int getM1 ();
    int getM2 ();
    int getM3 ();
    
    void configTimers ();
    void configMotorIO ();
    
    void stopMotors ();
    void setMotorSpeeds(int, int, int);
    void testMotors ();
    
    void calculateMotorSpeeds (double, double, double);
    void moveMachine (double, double, double);
    void moveMachineWithPID (double, double, double);
    void calibrateMachine();
    void motorsDirectionCheck(uint16_t b_speed);
    
    void setPID (float, float, float);
    float getZSum ();
};
