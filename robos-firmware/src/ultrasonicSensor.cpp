#include "ultrasonicSensor.h"

const int trigPin = 5;
const int echoPin = 18;

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void ultrasonic_setup(){
    Serial.println("HC-SR04 Sensor!");
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void ultrasonic_loop(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    distanceCm = duration * SOUND_SPEED/2;
    distanceInch = distanceCm * CM_TO_INCH;
}

void ultrasonicBroadcastClientsTXT(WebSocketsServer &ws){
        /* Send via Websocket */
        String sensordata = "{"
                            "\"name\": \"HC-SR04\","
                            "\"distanceCM\": "+String(distanceCm)+","
                            "\"distanceInch\": "+String(distanceInch)+","
                            "}";
        ws.broadcastTXT(sensordata);
}

float getDistanceCm(){
    return distanceCm;
}