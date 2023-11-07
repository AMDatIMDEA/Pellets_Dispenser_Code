#include <AccelStepper.h>

#define shakeSteps 50
#define strokes 3
#define step 15
#define dispenseDiscretizations 30
#define shakeAcceleration 20000
#define dispenseAcceleration 10000

const int signalSize = 1;
char signal[signalSize];
int activeMotor = 8;

// Pin definitions for motor control
const int motor1Step = 3;
const int motor1Dir = 4;
const int motor2Step = 6;
const int motor2Dir = 7;
const int motor3Step = 9;
const int motor3Dir = 10;
const int motor4Step = 24;
const int motor4Dir = 26;
const int motor5Step = 30;
const int motor5Dir = 32;
const int motor6Step = 36;
const int motor6Dir = 38;
const int motor7Step = 42;
const int motor7Dir = 44;
const int motor8Step = 48;
const int motor8Dir = 50;

// Pin definitions for sleep control
const int sleepPin1 = 2; // Sleep pin for dispenser 1
const int sleepPin2 = 5; // Sleep pin for dispenser 2
const int sleepPin3 = 8; // Sleep pin for dispenser 3
const int sleepPin4 = 22; // Sleep pin for dispenser 4
const int sleepPin5 = 28; // Sleep pin for dispenser 5
const int sleepPin6 = 34; // Sleep pin for dispenser 6
const int sleepPin7 = 40; // Sleep pin for dispenser 7
const int sleepPin8 = 46; // Sleep pin for dispenser 8

AccelStepper steppers[8]; // Array to store all the stepper motor objects

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Motor pin configurations
  pinMode(motor1Step, OUTPUT);
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor2Step, OUTPUT);
  pinMode(motor2Dir, OUTPUT);
  pinMode(motor3Step, OUTPUT);
  pinMode(motor3Dir, OUTPUT);
  pinMode(motor4Step, OUTPUT);
  pinMode(motor4Dir, OUTPUT);
  pinMode(motor5Step, OUTPUT);
  pinMode(motor5Dir, OUTPUT);
  pinMode(motor6Step, OUTPUT);
  pinMode(motor6Dir, OUTPUT);
  pinMode(motor7Step, OUTPUT);
  pinMode(motor7Dir, OUTPUT);
  pinMode(motor8Step, OUTPUT);
  pinMode(motor8Dir, OUTPUT);

  // Sleep pin configurations
  pinMode(sleepPin1, OUTPUT);
  pinMode(sleepPin2, OUTPUT);
  pinMode(sleepPin3, OUTPUT);
  pinMode(sleepPin4, OUTPUT);
  pinMode(sleepPin5, OUTPUT);
  pinMode(sleepPin6, OUTPUT);
  pinMode(sleepPin7, OUTPUT);
  pinMode(sleepPin8, OUTPUT);

  // Set the maximum speed and acceleration of the stepper motors for each dispenser
  for (int i = 0; i < 8; i++) {
    steppers[i] = AccelStepper(AccelStepper::DRIVER, getMotorStepPin(i), getMotorDirPin(i));
    steppers[i].setMaxSpeed(1000);
  }
}

void loop() {
  
  if (Serial.available()) {
    activeMotor = Serial.read();
    activeMotor = activeMotor - '0';
    Serial.println("I received: ");
    Serial.println(activeMotor, DEC);
    for (int i = 0; i < 8; i++) {
      if (i != activeMotor) {
        setSleepPin(i, 0);
      } else if (i == activeMotor) {
        setSleepPin(i, 1);
      }
    }
  }
  if (activeMotor != 8) {
    steppers[activeMotor].setAcceleration(shakeAcceleration);
    for (int j = 0; j <= strokes; j++) {
      if (!Serial.available()) {
        steppers[activeMotor].move(shakeSteps);
        steppers[activeMotor].runToPosition();
      } else if (Serial.available()) {
        return;
      }
      if (!Serial.available()) {
        steppers[activeMotor].move(-shakeSteps);
        steppers[activeMotor].runToPosition();
      } else if (Serial.available()) {
        return;
      }
    }
    steppers[activeMotor].setAcceleration(dispenseAcceleration);
    for (int j = 0; j <= dispenseDiscretizations; j++) {
      if (!Serial.available()) {
        steppers[activeMotor].move(step);
        steppers[activeMotor].runToPosition();
      } else if (Serial.available()) {
        return;
      }
    }
  }
}

int getMotorStepPin(int dispenser) {
  // Map the dispenser number to the corresponding motor step pin
  switch (dispenser) {
    case 0:
      return motor1Step;
    case 1:
      return motor2Step;
    case 2:
      return motor3Step;
    case 3:
      return motor4Step;
    case 4:
      return motor5Step;
    case 5:
      return motor6Step;
    case 6:
      return motor7Step;
    case 7:
      return motor8Step;
    default:
      break;
  }
}

int getMotorDirPin(int dispenser) {
  // Map the dispenser number to the corresponding motor direction pin
  switch (dispenser) {
    case 0:
      return motor1Dir;
    case 1:
      return motor2Dir;
    case 2:
      return motor3Dir;
    case 3:
      return motor4Dir;
    case 4:
      return motor5Dir;
    case 5:
      return motor6Dir;
    case 6:
      return motor7Dir;
    case 7:
      return motor8Dir;
    default:
      break;
  }
}

void setSleepPin(int dispenser, bool value) {
  // Map the dispenser number to the corresponding sleep pin
  switch (dispenser) {
    case 0:
      digitalWrite(sleepPin1, value);
      break;
    case 1:
      digitalWrite(sleepPin2, value);
      break;
    case 2:
      digitalWrite(sleepPin3, value);
      break;
    case 3:
      digitalWrite(sleepPin4, value);
      break;
    case 4:
      digitalWrite(sleepPin5, value);
      break;
    case 5:
      digitalWrite(sleepPin6, value);
      break;
    case 6:
      digitalWrite(sleepPin7, value);
      break;
    case 7:
      digitalWrite(sleepPin8, value);
      break;
    default:
      break;
  }
}