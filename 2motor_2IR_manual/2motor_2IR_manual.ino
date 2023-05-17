#include <AccelStepper.h>

#define SENSOR_PIN_A 7
#define SENSOR_PIN_B 12


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

AccelStepper stepperA(AccelStepper::DRIVER, stepPinA, 0); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper stepperB(AccelStepper::DRIVER, stepPinB, 0); // (Type of driver: with 2 pins, STEP, DIR)

unsigned long startTimeA; 
unsigned long stopTimeA;
unsigned long startTimeB;
unsigned long stopTimeB;
unsigned long previousMillis = 0;
const long interval = 3;
unsigned long changeRotation = 0;


unsigned long timeA;
unsigned long timeB;

unsigned long startTime = 0;

void setup() { 
  Serial.begin(9600); 

  pinMode(SENSOR_PIN_A, INPUT); 
  digitalWrite(SENSOR_PIN_A, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_B, INPUT); 
  digitalWrite(SENSOR_PIN_B, HIGH); // turn on the pullup

  stepperA.setMaxSpeed(1000);
  stepperB.setMaxSpeed(1000);
  stepperA.setSpeed(350);
  stepperB.setSpeed(100);

  startTime = millis();  // Enregistre le temps de départ initial

  
} 

void loop() { 

  // read the state of the sensor A and B value:
  sensorStateA = digitalRead(SENSOR_PIN_A);
  sensorStateB = digitalRead(SENSOR_PIN_B);

  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;


if ( elapsedTime > 1000){
    startTime = currentTime;

    stepperA.setSpeed(0);  
    stepperA.runSpeed();
    delay(1000);
    stepperA.move(-500);
    stepperA.runToPosition();
    delay(5000);
    }  

  if (sensorStateA && !lastStateA) {
    Serial.println("Unbroken");
    sensorStateA = digitalRead(SENSOR_PIN_A);
    sensorStateB = digitalRead(SENSOR_PIN_B);
    stepperA.runSpeed();
    stepperB.runSpeed(); 
    } 

  if (!sensorStateA && lastStateA) {
    //Serial.println("broken");
    
    startTimeA = millis();

    Serial.println(startTimeA);

    sensorStateA = digitalRead(SENSOR_PIN_A);
    sensorStateB = digitalRead(SENSOR_PIN_B);

    while(!sensorStateA && lastStateA) {
        sensorStateA = digitalRead(SENSOR_PIN_A);
        sensorStateB = digitalRead(SENSOR_PIN_B);
        stepperA.runSpeed();
        stepperB.runSpeed();  
    }
    stopTimeA = millis();
    timeA = stopTimeA - startTimeA;
    Serial.println(timeA);

    if (timeA < 3) {
      // Ignore small disturbances
    } else if (timeA >= 3 && timeA <= 15) {
      pelletsA++;
      Serial.println(timeA);
    } else if (timeA > 15) {
      pelletsA += 2;
      Serial.println(timeA);
    }
    
    if (startTimeA > 1000){
      Serial.println("Yo");
      stepperA.setSpeed(0);
      stepperA.runSpeed(); 
      delay(1000);
      stepperA.setSpeed(-250);
      stepperA.runSpeed();
      delay(1000);
      changeRotation = millis();
    }   
    if (millis()- changeRotation > 1000){
      stepperA.setSpeed(250);
    }   
    
  }

    // Détection de la bille pour le moteur B
    if (!sensorStateB && lastStateB) {
      startTimeB = millis();
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);

      while (!sensorStateB && lastStateB) {
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          stepperA.runSpeed();
          stepperB.runSpeed();      
        }

      stopTimeB = millis();
      timeB = stopTimeB - startTimeB;
      Serial.println(timeB);

      if (timeB < 3) {
        // Ne rien faire
      } else if (timeB >= 3 && timeB <= 15) {
        pelletsB++;
        Serial.println(timeB);
      } else if (timeB > 15) {
        pelletsB += 2;
        Serial.println(timeB);
      }
    }
    stepperA.runSpeed();
    stepperB.runSpeed();

    lastStateA = sensorStateA;
    lastStateB = sensorStateB;

}

