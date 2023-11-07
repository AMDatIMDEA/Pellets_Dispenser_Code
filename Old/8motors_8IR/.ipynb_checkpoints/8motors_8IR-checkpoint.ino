// Library to use the motor
#include <AccelStepper.h>

// Definition of pin of all the sensor
#define SENSOR_PIN_A 7
#define SENSOR_PIN_B 12
#define SENSOR_PIN_C 13
#define SENSOR_PIN_D 14
#define SENSOR_PIN_E 15
#define SENSOR_PIN_F 16
#define SENSOR_PIN_G 17
#define SENSOR_PIN_H 18



// Variables to see the difference when a pellet is seen by a sensor
int sensorStateA = 0, lastStateA =0;
int sensorStateB = 0, lastStateB =0;
int sensorStateC = 0, lastStateC =0;
int sensorStateD = 0, lastStateD =0;
int sensorStateE = 0, lastStateE =0;
int sensorStateF = 0, lastStateF =0;
int sensorStateG = 0, lastStateG =0;
int sensorStateH = 0, lastStateH =0;

// Definition of pin of all the motor
const int stepPinA = 8;
const int stepPinB = 9;
const int stepPinC = 9; //check what is free
const int stepPinD = 9;
const int stepPinE = 9;
const int stepPinF = 9;
const int stepPinG = 9;
const int stepPinH = 9;

// Variable for reading informations send by python
const int MSG_SIZE = 7;
char msg[MSG_SIZE];

// Variables that retrieve information about the number of pellets sent by python 
int pelletsA = 0;
int pelletsB = 0;
int pelletsC = 0;
int pelletsD = 0;
int pelletsE = 0;
int pelletsF = 0;
int pelletsG = 0;
int pelletsH = 0;

// Variables that retrieve information about the speed of the motors sent by python 
int speedA = 0;
int speedB = 0;
int speedC = 0;
int speedD = 0;
int speedE = 0;
int speedF = 0;
int speedG = 0;
int speedH = 0;

// Variables that retrieve information about the number of pellets sent by python 
int nbA = 0;
int nbB = 0;
int nbC = 0;
int nbD = 0;
int nbE = 0;
int nbF = 0;
int nbG = 0;
int nbH = 0;

// Fonction who inizialise a Steppermotor with 2 wires, command pin  and direction
AccelStepper stepperA(AccelStepper::DRIVER, stepPinA, 0); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper stepperB(AccelStepper::DRIVER, stepPinB, 0);
AccelStepper stepperC(AccelStepper::DRIVER, stepPinC, 0);
AccelStepper stepperD(AccelStepper::DRIVER, stepPinD, 0);
AccelStepper stepperE(AccelStepper::DRIVER, stepPinE, 0);
AccelStepper stepperF(AccelStepper::DRIVER, stepPinF, 0);
AccelStepper stepperG(AccelStepper::DRIVER, stepPinG, 0);
AccelStepper stepperH(AccelStepper::DRIVER, stepPinH, 0);

// Variables uses to know the time when a pellet is seen by a sensor
unsigned long startTimeA; 
unsigned long startTimeB;
unsigned long startTimeC;
unsigned long startTimeD;
unsigned long startTimeE;
unsigned long startTimeF;
unsigned long startTimeG;
unsigned long startTimeH;

// Variables uses to know when the sensor don't see anymore the pellet seen with startTime
unsigned long stopTimeA;
unsigned long stopTimeB;
unsigned long stopTimeC;
unsigned long stopTimeD;
unsigned long stopTimeE;
unsigned long stopTimeF;
unsigned long stopTimeG;
unsigned long stopTimeH;

// Variables uses to know the time that a sensor has seen a pellets with the 2 previous variables
unsigned long timeA;
unsigned long timeB;
unsigned long timeC;
unsigned long timeD;
unsigned long timeE;
unsigned long timeF;
unsigned long timeG;
unsigned long timeH;

// In progress
unsigned long previousMillis = 0;
const long interval = 3;
unsigned long changeRotation = 0;

void setup() { 
  Serial.begin(9600); 

// Initialization of the sensors
  pinMode(SENSOR_PIN_A, INPUT); 
  digitalWrite(SENSOR_PIN_A, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_B, INPUT); 
  digitalWrite(SENSOR_PIN_B, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_C, INPUT); 
  digitalWrite(SENSOR_PIN_C, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_D, INPUT); 
  digitalWrite(SENSOR_PIN_D, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_E, INPUT); 
  digitalWrite(SENSOR_PIN_E, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_F, INPUT); 
  digitalWrite(SENSOR_PIN_F, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_G, INPUT); 
  digitalWrite(SENSOR_PIN_G, HIGH); // turn on the pullup

  pinMode(SENSOR_PIN_H, INPUT); 
  digitalWrite(SENSOR_PIN_H, HIGH); // turn on the pullup

  // Initialization of the speed of the motor
  stepperA.setMaxSpeed(1000);
  stepperB.setMaxSpeed(1000);
  stepperC.setMaxSpeed(1000);
  stepperD.setMaxSpeed(1000);
  stepperE.setMaxSpeed(1000);
  stepperF.setMaxSpeed(1000);
  stepperG.setMaxSpeed(1000);
  stepperH.setMaxSpeed(1000);
  
  stepperA.setSpeed(speedA);
  stepperB.setSpeed(speedB);
  stepperC.setSpeed(speedC);
  stepperD.setSpeed(speedD);
  stepperE.setSpeed(speedE);
  stepperF.setSpeed(speedF);
  stepperG.setSpeed(speedG);
  stepperH.setSpeed(speedH);
} 

void loop() { 
    // Reception of data sent by arduino
  if (Serial.available() > 0) {
    int rlen1 = Serial.readBytes(msg, MSG_SIZE);

    if (msg[0] == 'a') {
      speedA = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperA.setSpeed(speedA);
      nbA = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    } 
    else if (msg[0] == 'b') {
      speedB = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperB.setSpeed(speedB);
      nbB = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }
    else if (msg[0] == 'c') {
      speedC = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperC.setSpeed(speedC);
      nbC = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }
    else if (msg[0] == 'd') {
      speedD = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperD.setSpeed(speedD);
      nbD = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }
    else if (msg[0] == 'e') {
      speedE = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperE.setSpeed(speedE);
      nbE = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }
    else if (msg[0] == 'f') {
      speedF = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperF.setSpeed(speedF);
      nbF = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }
    else if (msg[0] == 'g') {
      speedG = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperG.setSpeed(speedG);
      nbG = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }
    else if (msg[0] == 'h') {
      speedH = (msg[1]-'0')*100 + (msg[2]-'0')*10 + (msg[3]-'0');
      stepperH.setSpeed(speedH);
      nbH = (msg[4]-'0')*1000 + (msg[5]-'0')*100 + (msg[6]-'0')*10 + (msg[7]-'0');
    }

  }

  // Step the motors with the constant speeds previously set by setSpeed();
  if (speedA > 1) {
    stepperA.runSpeed();
  }
  if (speedB > 1) {
    stepperB.runSpeed();
  }
  if (speedC > 1) {
    stepperC.runSpeed();
  }
  if (speedD > 1) {
    stepperD.runSpeed();
  }
  if (speedE > 1) {
    stepperE.runSpeed();
  }
  if (speedF > 1) {
    stepperF.runSpeed();
  }
  if (speedG > 1) {
    stepperG.runSpeed();
  }
  if (speedH > 1) {
    stepperH.runSpeed();
  }  

  // read the state of the 8 sensor:
  sensorStateA = digitalRead(SENSOR_PIN_A);
  sensorStateB = digitalRead(SENSOR_PIN_B);
  sensorStateC = digitalRead(SENSOR_PIN_C);
  sensorStateD = digitalRead(SENSOR_PIN_D);
  sensorStateE = digitalRead(SENSOR_PIN_E);
  sensorStateF = digitalRead(SENSOR_PIN_F);
  sensorStateG = digitalRead(SENSOR_PIN_G);
  sensorStateH = digitalRead(SENSOR_PIN_H);

  // Variables to make verification (in progress)
  unsigned long currentMillis = millis();
  unsigned long lastDetection = millis();
  Serial. println(lastDetection);  

if (lastDetection > 1000){
    stepperA.setSpeed(0);      
    delay(1000);
    stepperA.setSpeed(-250);
    delay(1000);
    lastDetection= millis();
    changeRotation = millis();
    }  

  // Verification if the sensor A has seen a pellets
  if (sensorStateA && !lastStateA) {
    Serial.println("Unbroken");

    // read the state of the 8 sensor:
    sensorStateA = digitalRead(SENSOR_PIN_A);
    sensorStateB = digitalRead(SENSOR_PIN_B);
    sensorStateC = digitalRead(SENSOR_PIN_C);
    sensorStateD = digitalRead(SENSOR_PIN_D);
    sensorStateE = digitalRead(SENSOR_PIN_E);
    sensorStateF = digitalRead(SENSOR_PIN_F);
    sensorStateG = digitalRead(SENSOR_PIN_G);
    sensorStateH = digitalRead(SENSOR_PIN_H); 
    } 

  if (!sensorStateA && lastStateA) {
    //Serial.println("broken");
    startTimeA = millis();
    // read the state of the 8 sensor:
    sensorStateA = digitalRead(SENSOR_PIN_A);
    sensorStateB = digitalRead(SENSOR_PIN_B);
    sensorStateC = digitalRead(SENSOR_PIN_C);
    sensorStateD = digitalRead(SENSOR_PIN_D);
    sensorStateE = digitalRead(SENSOR_PIN_E);
    sensorStateF = digitalRead(SENSOR_PIN_F);
    sensorStateG = digitalRead(SENSOR_PIN_G);
    sensorStateH = digitalRead(SENSOR_PIN_H);


    while(!sensorStateA && lastStateA) {
        // read the state of the 8 sensor:
        sensorStateA = digitalRead(SENSOR_PIN_A);
        sensorStateB = digitalRead(SENSOR_PIN_B);
        sensorStateC = digitalRead(SENSOR_PIN_C);
        sensorStateD = digitalRead(SENSOR_PIN_D);
        sensorStateE = digitalRead(SENSOR_PIN_E);
        sensorStateF = digitalRead(SENSOR_PIN_F);
        sensorStateG = digitalRead(SENSOR_PIN_G);
        sensorStateH = digitalRead(SENSOR_PIN_H);
    }
    stopTimeA = millis();
    timeA = stopTimeA - startTimeA;
    Serial.println(timeA);
    lastDetection = millis();

      // Count the pellets according to the detection time of the sensor
    if (timeA < 3) {
      // Ignore small disturbances
    } else if (timeA >= 3 && timeA <= 15) {
      pelletsA++;
    } else if (timeA > 15) {
      pelletsA += 2;
    }
    
    if (startTimeA > 1000){
      Serial.println("Yo");
      stepperA.setSpeed(0);      
      delay(1000);
      stepperA.setSpeed(-250);
      delay(1000);
      changeRotation = millis();
    }   
    if (millis()- changeRotation > 1000){
      stepperA.setSpeed(250);
    }   
    
  }

  // Verification if the sensor B has seen a pellets
    if (!sensorStateB && lastStateB) {
      startTimeB = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateB && lastStateB) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeB = millis();
      timeB = stopTimeB - startTimeB;
      Serial.println(timeB);

      // Count the pellets according to the detection time of the sensor
      if (timeB < 3) {
        // Ne rien faire
      } else if (timeB >= 3 && timeB <= 15) {
        pelletsB++;
      } else if (timeB > 15) {
        pelletsB += 2;
      }
    }

  // Verification if the sensor C has seen a pellets
    if (!sensorStateC && lastStateC) {
      startTimeC = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateC && lastStateC) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeC = millis();
      timeC = stopTimeC - startTimeC;
      Serial.println(timeC);

      // Count the pellets according to the detection time of the sensor
      if (timeC < 3) {
        // Ne rien faire
      } else if (timeC >= 3 && timeC <= 15) {
        pelletsC++;
      } else if (timeC > 15) {
        pelletsC += 2;
      }
    }

  // Verification if the sensor D has seen a pellets
    if (!sensorStateD && lastStateD) {
      startTimeD = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateD && lastStateD) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeD = millis();
      timeD = stopTimeD - startTimeD;
      Serial.println(timeD);
      
      // Count the pellets according to the detection time of the sensor
      if (timeD < 3) {
        // Ne rien faire
      } else if (timeD >= 3 && timeD <= 15) {
        pelletsD++;
      } else if (timeD > 15) {
        pelletsD += 2;
      }
    }

  // Verification if the sensor E has seen a pellets
    if (!sensorStateE && lastStateE) {
      startTimeE = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateE && lastStateE) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeE = millis();
      timeE = stopTimeE - startTimeE;
      Serial.println(timeE);

      // Count the pellets according to the detection time of the sensor
      if (timeE < 3) {
        // Ne rien faire
      } else if (timeE >= 3 && timeE <= 15) {
        pelletsE++;
      } else if (timeE > 15) {
        pelletsE += 2;
      }
    }

  // Verification if the sensor F has seen a pellets
    if (!sensorStateF && lastStateF) {
      startTimeF = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateF && lastStateF) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeF = millis();
      timeF = stopTimeF - startTimeF;
      Serial.println(timeF);

      // Count the pellets according to the detection time of the sensor
      if (timeF < 3) {
        // Ne rien faire
      } else if (timeF >= 3 && timeF <= 15) {
        pelletsF++;
      } else if (timeF > 15) {
        pelletsF += 2;
      }
    }

  // Verification if the sensor G has seen a pellets
    if (!sensorStateG && lastStateG) {
      startTimeG = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateG && lastStateG) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeG = millis();
      timeG = stopTimeG - startTimeG;
      Serial.println(timeG);

      // Count the pellets according to the detection time of the sensor
      if (timeG < 3) {
        // Ne rien faire
      } else if (timeG >= 3 && timeG <= 15) {
        pelletsG++;
      } else if (timeG > 15) {
        pelletsG += 2;
      }
    }

  // Verification if the sensor H has seen a pellets
    if (!sensorStateH && lastStateH) {
      startTimeH = millis();
      // read the state of the 8 sensor:
      sensorStateA = digitalRead(SENSOR_PIN_A);
      sensorStateB = digitalRead(SENSOR_PIN_B);
      sensorStateC = digitalRead(SENSOR_PIN_C);
      sensorStateD = digitalRead(SENSOR_PIN_D);
      sensorStateE = digitalRead(SENSOR_PIN_E);
      sensorStateF = digitalRead(SENSOR_PIN_F);
      sensorStateG = digitalRead(SENSOR_PIN_G);
      sensorStateH = digitalRead(SENSOR_PIN_H);

      while (!sensorStateH && lastStateH) {
          // read the state of the 8 sensor:
          sensorStateA = digitalRead(SENSOR_PIN_A);
          sensorStateB = digitalRead(SENSOR_PIN_B);
          sensorStateC = digitalRead(SENSOR_PIN_C);
          sensorStateD = digitalRead(SENSOR_PIN_D);
          sensorStateE = digitalRead(SENSOR_PIN_E);
          sensorStateF = digitalRead(SENSOR_PIN_F);
          sensorStateG = digitalRead(SENSOR_PIN_G);
          sensorStateH = digitalRead(SENSOR_PIN_H);   
        }

      stopTimeH = millis();
      timeH = stopTimeH - startTimeH;
      Serial.println(timeH);

      // Count the pellets according to the detection time of the sensor
      if (timeH < 3) {
        // Ne rien faire
      } else if (timeH >= 3 && timeH <= 15) {
        pelletsH++;
      } else if (timeH > 15) {
        pelletsH += 2;
      }
    }

    lastStateA = sensorStateA;
    lastStateB = sensorStateB;
    lastStateC = sensorStateC;
    lastStateD = sensorStateD;
    lastStateE = sensorStateE;
    lastStateF = sensorStateF;
    lastStateG = sensorStateG;
    lastStateH = sensorStateH;

}

