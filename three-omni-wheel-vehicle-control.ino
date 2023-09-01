#include <avr/io.h>
#include "Motor.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"
#include "MPU.h"
#define MASK(k) ((unsigned char)(1 << k))

/*=========================
Global variables
=========================*/
BaseMotor* bm;
bool flag = false;
uint16_t b_speed = 500;
bool toggleKi = false;

void displayMotorSpeeds(BaseMotor* bm) {
  Serial.print("M1: ");
  Serial.print(bm->getM1());
  Serial.print("\n");

  Serial.print("M2: ");
  Serial.print(bm->getM2());
  Serial.print("\n");

  Serial.print("M3: ");
  Serial.print(bm->getM3());
  Serial.print("\n");
}

void basicMotionWithApp(String key) {
  // up, down, left, right
  // CW and ACW
  if (key == "UP") {
    bm->moveMachine(b_speed - z, 0, 0);

  } else if (key == "DOWN") {
    bm->moveMachine(b_speed - z, 180, 0);

  } else if (key == "LEFT") {
    bm->moveMachine(b_speed - z, 90, 0);

  } else if (key == "RIGHT") {
    bm->moveMachine(b_speed - z, -90, 0);

  } else if (key == "UPRIGHT") {
    bm->moveMachine(b_speed - z, -45, 0);

  } else if (key == "UPLEFT") {
    bm->moveMachine(b_speed - z, 45, 0);

  } else if (key == "DOWNLEFT") {
    bm->moveMachine(b_speed - z, 45 + 90, 0);

  } else if (key == "DOWNRIGHT") {
    bm->moveMachine(b_speed - z, -45 - 90, 0);

  } else if (key == "CROSS") {
    bm->setMotorSpeeds(0, 0, 0);
    while (1)
      ;
  } else if (key == "CIRCLE") {
    bm->setMotorSpeeds(300, 300, 300);

  } else if (key == "SQUARE") {
    bm->setMotorSpeeds(-300, -300, -300);
  } else if (key == "TRI") {

  } else {
    bm->setMotorSpeeds(0, 0, 0);
  }
}


void basicMotionWithAppPID(String key) {
  // up, down, left, right
  // CW and ACW
  if (key == "UP") {
    bm->moveMachineWithPID(b_speed, 0, 0);

  } else if (key == "DOWN") {
    bm->moveMachineWithPID(b_speed, 180, 0);

  } else if (key == "LEFT") {
    bm->moveMachineWithPID(b_speed, 90, 0);

  } else if (key == "RIGHT") {
    bm->moveMachineWithPID(b_speed, -90, 0);

  } else if (key == "UPRIGHT") {
    bm->moveMachineWithPID(b_speed, -45, 0);

  } else if (key == "UPLEFT") {
    bm->moveMachineWithPID(b_speed, 45, 0);

  } else if (key == "DOWNLEFT") {
    bm->moveMachineWithPID(b_speed, 45 + 90, 0);

  } else if (key == "DOWNRIGHT") {
    bm->moveMachineWithPID(b_speed, -45 - 90, 0);

  } else if (key == "CROSS") {
    bm->setMotorSpeeds(0, 0, 0);
    while (1)
      ;

  } else if (key == "CIRCLE") {
    bm->setMotorSpeeds(300, 300, 300);

  } else if (key == "SQUARE") {
    bm->setMotorSpeeds(-300, -300, -300);

  } else if (key == "TRI") {
    set_mpu();
    delay(100);
  } else {
    bm->moveMachineWithPID(0, 0, z);
  }
}
/**
   Globally defined keys
*/

String keys[] = {
  "UP",
  "DOWN",
  "LEFT",
  "RIGHT",
  "UPRIGHT",
  "UPLEFT",
  "DOWNRIGHT",
  "DOWNLEFT",
  "TRI",
  "CROSS",
  "SQUARE",
  "CIRCLE",
  "ZZZ_UP",
  "ZZZ_DOWN",
  "ZZZ_LEFT",
  "ZZZ_RIGHT",
  "ZZZ_TRI",
  "ZZZ_SQUARE",
  "ZZZ_CIRCLE",
  "ZZZ_SQUARE",
  "ZZZ_UPRIGHT",
  "ZZZ_UPLEFT",
  "ZZZ_DOWNRIGHT",
  "ZZZ_DOWNLEFT"
};

bool filterData(String key) {
  const int N = 24;
  for (int i = 0; i < N; ++i) {
    if (key == keys[i]) {
      return true;
    }
  }

  const int len = key.length();
  if (key[0] == "<" && key[len - 1] == ">") return true;

  return false;
}

void setup() {
  bm = new BaseMotor();
  Serial.begin(115200);
  Serial2.begin(74880);
  // set_mpu();
  // printParams();
}

void loop() {
  // pid_mpu(); // update MPU data
  // printParams();
  // delay(100);
  // bm->setPID(P, I, D); // update P, I, D parameter
  // bm->moveMachineWithPID(0, 0, z);

  String key = "";
  while (Serial2.available()) {
    char c = Serial2.read();
    key += c;
  }

  if (key != ""
      && filterData(key)) {
    Serial.println(key);
    basicMotionWithApp(key);
    // basicMotionWithAppPID(key);
  }
  delay(10);
}
