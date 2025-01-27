//#include <Arduino.h>
//#include "DocykeMotor.h"

/**
-----------------
|               |
| 5  4  3  2  1 |
|               |
-----------------

5 - + 5v
4 - direction  --> 12
3 - speed      --> 13
2 - enable     --> 5
1 - - GND

**/

/*#define ENABLE_PIN 5
#define DIRECTION_PIN 12
#define VELOCITY_PIN 13

int joystick_x = 32;


// added a comment 

DocykeMotor motor(ENABLE_PIN, DIRECTION_PIN, VELOCITY_PIN); // Replace with your actual pin numbers

void setup()
{
  Serial.begin(115200);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(VELOCITY_PIN, OUTPUT);
  
  Serial.println("Starting motor");

  motor.enable();
  motor.setDirection(false); // Set to forward
  
}
// Comment Yoad

void loop()
{
  float x_value = (float)(analogRead(joystick_x)-2800)/5;
  Serial.println(x_value);
  motor.drive(x_value);
  /*
  Serial.println(!motor.getDirection()?"Forward":"Backwards");
  motor.drive(x_value);
  
}*/
