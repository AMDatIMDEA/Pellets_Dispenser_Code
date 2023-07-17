#include <AccelStepper.h>
#include <elapsedMillis.h>

// Pin definitions for IR sensors
const int irSensor1 = 2; // IR sensor input pin for dispenser 1
const int irSensor2 = 3; // IR sensor input pin for dispenser 2
const int irSensor3 = 4; // IR sensor input pin for dispenser 3
const int irSensor4 = 5; // IR sensor input pin for dispenser 4
const int irSensor5 = 6; // IR sensor input pin for dispenser 5
const int irSensor6 = 7; // IR sensor input pin for dispenser 6
const int irSensor7 = 8; // IR sensor input pin for dispenser 7
const int irSensor8 = 9; // IR sensor input pin for dispenser 8

// Pin definitions for motor control
const int motor1Step = 10;
const int motor1Dir = 11;
const int motor2Step = 12;
const int motor2Dir = 13;
const int motor3Step = 14;
const int motor3Dir = 15;
const int motor4Step = 16;
const int motor4Dir = 17;
const int motor5Step = 18;
const int motor5Dir = 19;
const int motor6Step = 20;
const int motor6Dir = 21;
const int motor7Step = 22;
const int motor7Dir = 23;
const int motor8Step = 24;
const int motor8Dir = 25;

// Pin definitions for sleep control
const int sleepPin1 = 26; // Sleep pin for dispenser 1
const int sleepPin2 = 27; // Sleep pin for dispenser 2
const int sleepPin3 = 28; // Sleep pin for dispenser 3
const int sleepPin4 = 29; // Sleep pin for dispenser 4
const int sleepPin5 = 30; // Sleep pin for dispenser 5
const int sleepPin6 = 31; // Sleep pin for dispenser 6
const int sleepPin7 = 32; // Sleep pin for dispenser 7
const int sleepPin8 = 33; // Sleep pin for dispenser 8

// Amount of pellets to be dispensed
int totalPellets = 0; // Total number of pellets to dispense

int dispensedPellets[8]; // Array to store the number of dispensed pellets for each dispenser

// Proportions for dispensing pellets
float proportions[8]; // Proportions for each dispenser (between 0 and 1)

// Velocities for dispensing pellets
int velocities[8]; // Velocities for each dispenser

// Shake variables
elapsedMillis shakeTimer; // Timer to track shaking interval
const unsigned long shakeInterval = 5000; // Shake interval in milliseconds

// Stepper motor initialization
AccelStepper steppers[8]; // Array to store all the stepper motor objects

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // IR sensor pin configurations
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(irSensor3, INPUT);
  pinMode(irSensor4, INPUT);
  pinMode(irSensor5, INPUT);
  pinMode(irSensor6, INPUT);
  pinMode(irSensor7, INPUT);
  pinMode(irSensor8, INPUT);

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

  // Wait for the user to input the proportions and total pellets via the serial monitor
  waitForUserInput();

  // Set the maximum speed and acceleration of the stepper motors for each dispenser
  for (int i = 0; i < 8; i++) {
    steppers[i] = AccelStepper(AccelStepper::DRIVER, getMotorStepPin(i), getMotorDirPin(i));
    steppers[i].setMaxSpeed(1000);
    steppers[i].setAcceleration(500);
    velocities[i] = proportions[i] * steppers[i].maxSpeed();
  }
}

void loop() {
  // Check the IR sensor inputs for each dispenser
  int sensorValues[8];
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = digitalRead(getIrSensorPin(i));
  }

  // Dispense pellets if required for each dispenser
  for (int i = 0; i < 8; i++) {
    if (proportions[i] > 0 && sensorValues[i] == HIGH && getDispensedPellets(i) < (totalPellets * proportions[i])) {
      steppers[i].setSpeed(velocities[i]);
      steppers[i].runSpeed();
      incrementDispensedPellets(i);
      setSleepPin(i, LOW); // Wake up the dispenser
    } else {
      steppers[i].setSpeed(0);
      steppers[i].runSpeed();
      setSleepPin(i, HIGH); // Put the dispenser to sleep
    }
  }

  // Shake the dispensers every 5 seconds if needed
  if (shouldShake()) {
    shakeDispensers();
  }

  // Stop dispensing when the desired number of pellets has been dispensed for all dispensers
  if (areAllPelletsDispensed()) {
    for (int i = 0; i < 8; i++) {
      steppers[i].setSpeed(0);
      steppers[i].runSpeed();
      setSleepPin(i, HIGH); // Put all dispensers to sleep
    }
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
  }
}

int getIrSensorPin(int dispenser) {
  // Map the dispenser number to the corresponding IR sensor pin
  switch (dispenser) {
    case 0:
      return irSensor1;
    case 1:
      return irSensor2;
    case 2:
      return irSensor3;
    case 3:
      return irSensor4;
    case 4:
      return irSensor5;
    case 5:
      return irSensor6;
    case 6:
      return irSensor7;
    case 7:
      return irSensor8;
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
  }
}

void incrementDispensedPellets(int dispenser) {
  dispensedPellets[dispenser]++;
}

int& getDispensedPellets(int dispenser) {
  // Get the reference to the dispensed pellets counter for the given dispenser
  // Adjust the array size if needed to match the number of dispensers
  static int dispensedPellets[8] = {0};
  return dispensedPellets[dispenser];
}

bool areAllPelletsDispensed() {
  // Check if the desired number of pellets has been dispensed for all dispensers
  for (int i = 0; i < 8; i++) {
    if (proportions[i] > 0 && getDispensedPellets(i) < (totalPellets * proportions[i])) {
      return false;
    }
  }
  return true;
}

void waitForUserInput() {
  Serial.println("Enter the proportions for each dispenser (8 values between 0 and 1, separated by spaces):");

  while (!Serial.available()) {
    // Wait for user input
  }

  // Read the user input for proportions from the serial monitor
  for (int i = 0; i < 8; i++) {
    while (!Serial.available()) {
      // Wait for user input
    }

    // Read the proportion value
    String input = Serial.readStringUntil(' ');
    proportions[i] = input.toFloat();
  }

  Serial.println("Enter the total number of pellets:");
  
  while (!Serial.available()) {
    // Wait for user input
  }

  // Read the user input for total pellets
  String input = Serial.readStringUntil('\n');
  totalPellets = input.toInt();
}

bool shouldShake() {
  // Check if the shake interval has passed
  return shakeTimer >= shakeInterval;
}

void shakeDispensers() {
  // Shake the dispensers by changing directions quickly
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 5; j++) {
      steppers[i].setAcceleration(50000);
      steppers[i].setSpeed(-velocities[i]);
      steppers[i].runSpeed();
      delay(50);
      steppers[i].setSpeed(-velocities[i]);
      steppers[i].runSpeed();
      delay(50);
    }
  steppers[i].setAcceleration(500);
  steppers[i].setSpeed(velocities[i]);
  }
  shakeTimer = 0; // Reset the shake timer
}