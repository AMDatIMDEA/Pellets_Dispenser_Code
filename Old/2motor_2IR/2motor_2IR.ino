#include <AccelStepper.h>

#define SENSOR_PIN_A 7
#define SENSOR_PIN_B 6

// variables will change:
int sensorStateA = 0, lastStateA =0;         // variable for reading the sensor status
// variables will change:
int sensorStateB = 0, lastStateB =0;         // variable for reading the sensor status

const int stepPinA = 8;
const int stepPinB = 9;

const int MSG_SIZE = 7;
char msg[MSG_SIZE];

int pelletsA = 0;
int pelletsB = 0;
int speedA = 0;
int speedB = 0;
int nbA = 0;
int nbB = 0;

AccelStepper stepperA(AccelStepper::DRIVER, stepPinA, 0); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper stepperB(AccelStepper::DRIVER, stepPinB, 0); // (Type of driver: with 2 pins, STEP, DIR)

unsigned long startTimeA; 
unsigned long stopTimeA;
unsigned long startTimeB;
unsigned long stopTimeB;
unsigned long previousMillis = 0;
const long interval = 3;

unsigned long timeA;
unsigned long timeB;

void setup() { 
  Serial.begin(9600); 

  pinMode(SENSOR_PIN_A, INPUT); 
  digitalWrite(SENSOR_PIN_A, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_B, INPUT); 
  digitalWrite(SENSOR_PIN_B, HIGH); // turn on the pullup



  stepperA.setMaxSpeed(1000);
  stepperB.setMaxSpeed(1000);
  stepperA.setSpeed(0);
  stepperB.setSpeed(0);
} 

void loop() { 

  // Reception des données envoyées par Python
  if (Serial.available() > 0) {
    int rlen1 = Serial.readBytes(msg, MSG_SIZE);

    if (msg[0] == 'a') {
      speedA = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperA.setSpeed(speedA);
      nbA = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    } else if (msg[0] == 'b') {
      speedB = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperB.setSpeed(speedB);
      nbB = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }

  }

  // Step the motors with the constant speeds previously set by setSpeed();
  if (speedA > 1) {
    stepperA.runSpeed();
  }
  if (speedB > 1) {
    stepperB.runSpeed();
  }

  // read the state of the sensor A and B value:
  sensorStateA = digitalRead(SENSOR_PIN_A);
  sensorStateB = digitalRead(SENSOR_PIN_B);

  unsigned long currentMillis = millis();

  if (sensorStateA && !lastStateA) {
    //Serial.println("Unbroken");
    sensorStateA = digitalRead(SENSOR_PIN_A);
    sensorStateB = digitalRead(SENSOR_PIN_B);
    stepperA.runSpeed();
  } 

  if (!sensorStateA && lastStateA) {
    //Serial.println("broken");
    startTimeA = millis();
    sensorStateA = digitalRead(SENSOR_PIN_A);
    sensorStateB = digitalRead(SENSOR_PIN_B);
    stepperA.runSpeed();

    while(!sensorStateA && lastStateA) {
        sensorStateA = digitalRead(SENSOR_PIN_A);
        sensorStateB = digitalRead(SENSOR_PIN_B);
        stepperA.runSpeed();
    }
    stopTimeA = millis();
    timeA = stopTimeA - startTimeA;
    //Serial.println(timeA);
    stepperA.runSpeed();

    if (timeA < 3) {
      // Ignore small disturbances
    } else if (timeA >= 3 && timeA <= 15) {
      pelletsA++;
      Serial.println(timeA);
    } else if (timeA > 15) {
      pelletsA += 2;
      Serial.println(timeA);
    }

    if(pelletsA==nbA){
      stepperA.setSpeed(0);      
    }
  }

    // Détection de la bille pour le moteur B
    if (!sensorStateB && lastStateB) {
      startTimeB = millis();
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
        stepperB.runSpeed();

      while (!sensorStateB && lastStateB) {
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          stepperB.runSpeed();
      }

      stopTimeB = millis();
      timeB = stopTimeB - startTimeB;
      stepperB.runSpeed();

      if (timeB < 3) {
        // Ne rien faire
      } else if (timeB >= 3 && timeB <= 15) {
        pelletsB++;
        Serial.println(timeB);
      } else if (timeB > 15) {
        pelletsB += 2;
        Serial.println(timeB);
      }

      if(pelletsB==nbB){
      stepperB.setSpeed(0);      
    }
    }
    stepperB.runSpeed();

    lastStateA = sensorStateA;
    lastStateB = sensorStateB;

}