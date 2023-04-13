#include "motorController.h"

const int motorLeftForward_pin = 26;
const int motorLeftBackward_pin = 27;
const int motorRightForward_pin = 23;
const int motorRightBackward_pin = 19;

int motorLeftPower = 0;
int motorRightPower = 0;

void motorController_setup(){
    Serial.println("Motor Controller!");
    pinMode(motorLeftForward_pin, OUTPUT);
    pinMode(motorLeftBackward_pin, OUTPUT);
    pinMode(motorRightForward_pin, OUTPUT);
    pinMode(motorRightBackward_pin, OUTPUT);
}

void motorController_loop(){

    if(motorLeftPower > 0){
        analogWrite(motorLeftForward_pin, motorLeftPower);
        analogWrite(motorLeftBackward_pin, 0);
    }else{
        analogWrite(motorLeftForward_pin, 0);
        analogWrite(motorLeftBackward_pin, -1*motorLeftPower);
    }

    if(motorRightPower > 0){
        analogWrite(motorRightForward_pin, motorRightPower);
        analogWrite(motorRightBackward_pin, 0);
    }else{
        analogWrite(motorRightForward_pin, 0);
        analogWrite(motorRightBackward_pin, -1*motorRightPower);
    }
    
}

int IRAM_ATTR bound(int x){
    if(x < -255){
        return -255;
    }
    if(x > 255){
        return 255;
    }
    return x;
}

void setMotorPower(int left, int right){
    motorLeftPower = bound(left);
    motorRightPower = bound(right);
    Serial.printf("Motor %d %d\n",motorLeftPower, motorRightPower);
}