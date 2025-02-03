#include <Arduino.h>
#include "DocykeMotor.h"
#include "PS2.cpp"

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

#define DATA_PIN 18
#define CMD_PIN 19
#define CLK_PIN 14
#define ATT_PIN 27

//Creating a motor object with the pin connections set above
DocykeMotor motor(ENABLE_PIN, DIRECTION_PIN, VELOCITY_PIN); // Replace with your actual pin numbers
Controller Controller1(DATA_PIN, CMD_PIN, CLK_PIN, ATT_PIN); //DATA, CMD, CLK, ATT

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup Complete");
}

int powers[]={195,205,215,225,235,245,255,-200,-240};

void loop()
{
  Serial.println(Controller1.circle);
  Serial.println(Controller1.left_joystick_X);
  
  Controller1.updateData();
}
