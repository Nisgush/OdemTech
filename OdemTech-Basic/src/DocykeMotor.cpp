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

uint8_t DocykeMotor::getVelPin(){
    return velocityPin;
}

/*
void dualDrive(DocykeMotor motor1, DocykeMotor motor2, int speed1, int speed2){
    if(speed1>255){
        speed1 = 255;
    } else if (speed1<-255){
        speed1 = -255;
    }
    if(speed1>=0 && !motor1.getDirection()){
        motor1.setDirection(false);
    }
    if(speed1>=0){
        analogWrite(motor1.getVelPin(),speed1);
    }
    if(speed1<0 && motor1.getDirection()){
        motor1.setDirection(true);
    }
    if(speed1<0){
        analogWrite(motor1.getVelPin(),-speed1);
    }
    if(speed2>255){
        speed2 = 255;
    } else if (speed2<-255){
        speed2 = -255;
    }
    if(speed2>=0 && !motor2.getDirection()){
        motor2.setDirection(false);
    }
    if(speed2>=0){
        analogWrite(motor2.getVelPin(),speed2);
    }
    if(speed2<0 && motor2.getDirection()){
        motor2.setDirection(true);
    }
    if(speed2<0){
        analogWrite(motor2.getVelPin(),-speed2);
    }
    delay(150);
    analogWrite(motor1.getVelPin(),0);
    analogWrite(motor2.getVelPin(),0);
}
    
*/
   
   void dualDrive(DocykeMotor motor1, DocykeMotor motor2, int speed1, int speed2) {
        // Clamp speed1 between -255 and 255
        speed1 = constrain(speed1, -255, 255);
        speed2 = constrain(speed2, -255, 255);
    
        // Set direction and speed for motor1
        if (speed1 < 0) {
            motor1.setDirection(true);  // Reverse
            analogWrite(motor1.getVelPin(), -speed1);  // Write positive PWM
        } else {
            motor1.setDirection(false);  // Forward
            analogWrite(motor1.getVelPin(), speed1);
        }
    
        // Set direction and speed for motor2
        if (speed2 < 0) {
            motor2.setDirection(true);  // Reverse
            analogWrite(motor2.getVelPin(), -speed2);  // Write positive PWM
        } else {
            motor2.setDirection(false);  // Forward
            analogWrite(motor2.getVelPin(), speed2);
        }
    
        // Run motors for a short time
        delay(150);
    
        // Stop motors after delay
        analogWrite(motor1.getVelPin(), 0);
        analogWrite(motor2.getVelPin(), 0);
    }
        
