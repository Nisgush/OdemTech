#include <Arduino.h>
#include "DocykeMotor.h"

/**
-----------------
|               |
| 5  4  3  2  1 |
|               |
-----------------

5 - + 5v
4 - direction  
3 - speed      
2 - enable    
1 - - GND

**/

#define ENABLE_PIN 5 //change to pin you connected
#define DIRECTION_PIN 12 //change to pin you connected
#define VELOCITY_PIN 13 //change to pin you connected

//Creating a motor object with the pin connections set above
DocykeMotor motor(ENABLE_PIN, DIRECTION_PIN, VELOCITY_PIN); // Replace with your actual pin numbers

void setup()
{
  Serial.begin(115200);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(VELOCITY_PIN, OUTPUT);
  
  Serial.println("Starting motor");

  motor.enable(); //Starting the motor
  motor.setDirection(false); // Set to forward
  
}

int powers[]={195,205,215,225,235,245,255};

void loop()
{
  for(int power:powers){//activates the motor with different powers for 1 second each
    motor.drive(power, 1000);
  }
}
