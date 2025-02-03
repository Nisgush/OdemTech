#include "DocykeMotor.h"

DocykeMotor::DocykeMotor(uint8_t enable, uint8_t direction, uint8_t velocity)
    : enablePin(enable), directionPin(direction), velocityPin(velocity){
    
    pinMode(enablePin, OUTPUT);
    pinMode(velocityPin, OUTPUT);
    pinMode(directionPin, OUTPUT);

    digitalWrite(enablePin, LOW);  // Pull low first
    delay(200);                     // Small delay for stability
    digitalWrite(enablePin, HIGH); // Then pull high
    
    setDirection(false);

    // Configure LEDC for PWM
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(velocityPin, pwmChannel);
    
    // Initial state
    digitalWrite(enablePin, LOW);
    digitalWrite(directionPin, LOW);
    ledcWrite(pwmChannel, 0);
}

void DocykeMotor::enable(){
    digitalWrite(enablePin, LOW);  // Pull low first
    delay(200);                     // Small delay for stability
    digitalWrite(enablePin, HIGH); // Then pull high
}

void DocykeMotor::setDirection(bool isBackward) {
    digitalWrite(directionPin, isBackward ? HIGH : LOW);
    motorDirection = !isBackward;
}

void DocykeMotor::setSpeed(uint8_t speed) {
    if (speed > 255) speed = 255;  // Ensure speed is within valid range
    ledcWrite(pwmChannel, speed);
}

bool DocykeMotor::getDirection() {
    return motorDirection;
}

void DocykeMotor::drive(int speed, int time){
    if(speed>255){
        speed = 255;
    } else if (speed<-255){
        speed = -255;
    }
    if(speed>=0 && !getDirection()){
        setDirection(false);
    }
    if(speed>=0){
        analogWrite(velocityPin,speed);
    }
    if(speed<0 && getDirection()){
        setDirection(true);
    }
    if(speed<0){
        analogWrite(velocityPin,-speed);
    }
    delay(time);
    analogWrite(velocityPin,0);
}

void DocykeMotor::drive(int speed){
    drive(speed, 10);
}
