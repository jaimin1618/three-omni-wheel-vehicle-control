#ifndef MOTOR_H
#define MOTOR_H
#include "Motor.h"
#define MASK(k) ((unsigned char)(1 << k))

/* NOTE: direction pin for motors 0 => CW, 1 => ACW */

BaseMotor::BaseMotor() {
  this->configTimers();   // config timers
  this->configMotorIO();  // config motor driver enable

  this->setupM1(0);
  this->setupM2(0);
  this->setupM3(0);

  this->setM1(this->getM1());
  this->setM2(this->getM2());
  this->setM3(this->getM3());
}

BaseMotor::BaseMotor(int m1, int m2, int m3) {
  this->configTimers();   // config timers
  this->configMotorIO();  // config motor driver enable

  this->setupM1(m1);
  this->setupM2(m2);
  this->setupM3(m3);

  this->setM1(this->getM1());
  this->setM2(this->getM2());
  this->setM3(this->getM3());
}


void BaseMotor::setM1(int m1) {
  this->setupM1(m1);
  if (m1 >= 0) {
    PORTH &= ~MASK(3);      // 6
    OCR4B = this->getM1();  // 7
    return;
  }
  PORTH |= MASK(3);
  OCR4B = (-1) * this->getM1();
}

void BaseMotor::setM3(int m3) {
  this->setupM3(m3);
  if (m3 >= 0) {
    PORTG &= ~MASK(5);      // 4
    OCR3A = this->getM3();  // 5
    return;
  }
  PORTG |= MASK(5);
  OCR3A = (-1) * this->getM3();
}

void BaseMotor::setM2(int m2) {
  this->setupM2(m2);
  if (m2 >= 0) {
    PORTH &= ~MASK(5);      // 8
    OCR1A = this->getM2();  // 11
    return;
  }
  PORTH |= MASK(5);
  OCR1A = (-1) * this->getM2();
}

int BaseMotor::getM1() {
  return this->M1;
}
int BaseMotor::getM2() {
  return this->M2;
}
int BaseMotor::getM3() {
  return this->M3;
}

void BaseMotor::stopMotors() {
  this->setM1(0);
  this->setM2(0);
  this->setM3(0);
}

void BaseMotor::configTimers() {
  /*======================================
  Using timer 1, 3, 4 => CR1A, OCR3A, OCR4B
  ======================================*/

  /*======================================
  Using FAST PWM with ICR mode-14
  WGMi0=0, WGMi1=1, WGMi2=1, WGMi3=1
  8 bit prescalling CSn0=0, CSn1=1, CSn2=0
  ======================================*/
  TCCR1A = (1 << WGM11) | (1 << COM1A1);
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);

  TCCR3A = (1 << WGM31) | (1 << COM3A1);
  TCCR3B = (1 << WGM32) | (1 << WGM33) | (1 << CS31);

  TCCR4A = (1 << WGM41) | (1 << COM4B1);
  TCCR4B = (1 << WGM42) | (1 << WGM43) | (1 << CS41);

  // setting ICR
  ICR1 = 800;
  ICR3 = 800;
  ICR4 = 800;
}

void BaseMotor::configMotorIO() {
  // MAKING direction pins & OCR pins as an output
  DDRH |= MASK(3) | MASK(5) | MASK(4);
  DDRG |= MASK(5);
  DDRE |= MASK(3);
  DDRB |= MASK(5);
}

void BaseMotor::setMotorSpeeds(int m1, int m2, int m3) {
  this->setupM1(m1);
  this->setupM2(m2);
  this->setupM3(m3);

  this->setM1(this->getM1());
  this->setM2(this->getM2());
  this->setM3(this->getM3());
}

void BaseMotor::setupM1(int m1) {
  this->M1 = m1;
}

void BaseMotor::setupM2(int m2) {
  this->M2 = m2;
}

void BaseMotor::setupM3(int m3) {
  this->M3 = m3;
}

void BaseMotor::testMotors() {
  for (int i = 0; i <= 2000; i += 100) {
    this->setMotorSpeeds(i, i, i);
    _delay_ms(5000);
  }
}


void BaseMotor::calculateMotorSpeeds(double baseSpeed, double degree, double W = 0) {
  degree += 180;
  double rad = M_PI * degree / 180;
  double Vx = (-1) * baseSpeed * sin(rad);
  double Vy = baseSpeed * cos(rad);
  double M1, M2, M3;

  /*======================================

  Implementing Math - for getting motor speeds
  M1 = (-Vx / 3) + ((1 / sqrt(3)) * Vy) + (W / 3);
  M2 = (-Vx / 3) + ((-1 / sqrt(3)) * Vy) + (W / 3);
  M3 = (2 * Vx / 3) + (0 * Vy) + (W / 3);
  
  ======================================*/

  M1 = (-2 * Vx / 3) + (0 * Vy) + (W / 3);
  M2 = (Vx / 3) + (-Vy / sqrt(3)) + (W / 3);
  M3 = (Vx / 3) + (Vy / sqrt(3)) + (W / 3);

  this->setupM1(M1);
  this->setupM2(M2);
  this->setupM3(M3);
}

void BaseMotor::moveMachine(double baseSpeed, double degree, double W = 0) {
  this->calculateMotorSpeeds(baseSpeed, degree, W);
  this->setMotorSpeeds(this->getM1(), this->getM2(), this->getM3());
}

void BaseMotor::moveMachineWithPID(double baseSpeed, double degree, double W = 0) {
  this->calculateMotorSpeeds(baseSpeed, degree, W);
  this->setMotorSpeeds(
    ((float)this->getM1() - (float)this->Z_SUM),
    ((float)this->getM2() - (float)this->Z_SUM),
    ((float)this->getM3() - (float)this->Z_SUM));
}

void BaseMotor::setPID(float p, float i, float d) {
  this->P = p;
  this->I = i;
  this->D = d;
  this->Z_SUM = this->P + this->I + this->D;
}

float BaseMotor::getZSum() {
  return this->Z_SUM;
}

void BaseMotor::calibrateMachine() {
  this->moveMachineWithPID(0, 0, this->getZSum());
}

void BaseMotor::motorsDirectionCheck(uint16_t b_speed) {
  this->setMotorSpeeds(
    b_speed / 2,
    b_speed / 2,
    b_speed / 2);
}

/*======================================
Identifying IO operations and pins

1) direction & OCR pins - NOTE:
    D4 => PORTG(5)
    D5 => PORTE(3)
    D6 => PORTH(3)
    D7 => PORTH(4)
    D8 => PORTH(5)
    D11 => PORTB(5)

2) Motor
    MOTOR 3 => 4, 5(PWM)
    MOTOR 1 => 6, 7(PWM)
    MOTOR 2 => 8, 11(PWM)

======================================*/

#endif
