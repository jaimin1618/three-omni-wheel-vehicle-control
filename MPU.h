#define MPU_INIT 1

#ifndef PS2_INIT

#endif

#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

MPU6050 mpu;

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float euler[3];
float ypr[3];
long tm = 0;

volatile bool mpuInterrupt = false;

unsigned long curr = 0, prev = 0;
double dt = 0;
float previous_error = 0, e;
float P = 0;
float I = 0, D = 0, z = 0;

float yaw = 0;
float reference = 0;
float desired = 0;

float kp = 70, kd = 0, ki = 0.0;
// Listing Pair of Kp, Kd, Ki
// 20, 1, 0 => Working perfect
// 10, 3, 0.06 => TESTING
int first_time = 1;

void dmpDataReady() {
    mpuInterrupt = true;
}

void printLine(char str[], float value) {
    Serial.print(str);
    Serial.print(value);
    Serial.print("\n");
}

void printParams () {
    Serial.println("###### PID DATA ######");
    printLine("Yaw (actual radian error): ", yaw);
    printLine("reference: ", reference);
    printLine("Error(e): ", e);
    printLine("Desired: ", desired);
    printLine("Kp: ", kp);
    printLine("Kd: ", kd);
    printLine("Ki: ", ki);
    printLine("P: ", P);
    printLine("I: ", I);
    printLine("D: ", D);
    printLine("Z: ", z);
    Serial.println("###### END PID DATA ######");
}

float return_yaw() {
    while (!mpuInterrupt && fifoCount < packetSize);
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        mpu.resetFIFO();
        return (ypr[0] * 180 / M_PI);
    }
}

float ret_yaw() {
  //  Serial.println(millis() - tm);
    //  tm = millis();
    float temper = return_yaw();
    if (reference >= 0 && reference <= 180) {
        return ((temper - reference) < -180 ? temper - reference + 360 : temper - reference);
    } else if (reference >= -180 && reference < 0) {
        return ((temper - reference) > 180 ? temper - reference - 360 : temper - reference);
    }
}
void reset_para() {
    yaw = e = previous_error = P = I = D = z = reference = 0;
}

void set_mpu()
{
  int kam = 0;
  if (first_time == 1)
  {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    digitalWrite(SDA, LOW);
    digitalWrite(SCL, LOW);
    //    TWBR = 24;
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(110);
    mpu.setYGyroOffset(20);
    mpu.setZGyroOffset(81);
    
    mpu.setXAccelOffset(-1560);
    mpu.setYAccelOffset(-2273);
    mpu.setZAccelOffset(1799);
//    Your offsets:    -1560   -2273   1799    110 20  81


    if (devStatus == 0) {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(19), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        while (1);
        Serial.println(F(")"));
    }
    
    kam = 0;
    while (kam < 100) {
        float temp = ret_yaw();
        //      ps2_data();
        kam++;
        Serial.println(kam);
    }
    reference = ret_yaw();
    first_time = 0;
  }
  else {
    kam = 0;
    reset_para();
    while (kam < 5)
    {
      float temp = ret_yaw();
      kam++;
    }
    reference = ret_yaw();
  }
}

void pid_mpu() {
    yaw = ret_yaw();
    // Serial.println(yaw);
    e = yaw;
    P = kp * e;
    I += ki * e ;
    D = kd * (e - previous_error);
    z = P + I + D;
    previous_error = e;
}
