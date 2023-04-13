#include "encoderSensor.h"

const int frontleft_encoder_pin = 16;
const int frontright_encoder_pin = 4;
const int backleft_encoder_pin = 2;
const int backright_encoder_pin = 15;

int frontleft_steps = 0;
int frontright_steps = 0;
int backleft_steps = 0;
int backright_steps = 0;

void IRAM_ATTR increment_frontleft_steps(){
    frontleft_steps++;
}

void IRAM_ATTR increment_frontright_steps(){
    frontright_steps++;
}

void IRAM_ATTR increment_backleft_steps(){
    backleft_steps++;
}

void IRAM_ATTR increment_backright_steps(){
    backright_steps++;
}

void encoder_setup(){
    Serial.println("LM393 Sensor!");
    pinMode(frontleft_encoder_pin, INPUT);
    pinMode(frontright_encoder_pin, INPUT);
    pinMode(backleft_encoder_pin, INPUT);
    pinMode(backright_encoder_pin, INPUT);
    
    attachInterrupt(frontleft_encoder_pin, increment_frontleft_steps, RISING);
    attachInterrupt(frontright_encoder_pin, increment_frontright_steps, RISING);
    attachInterrupt(backleft_encoder_pin, increment_backleft_steps, RISING);
    attachInterrupt(backright_encoder_pin, increment_backright_steps, RISING);
}

void encoder_loop(){
}

void encoderBroadcastClientsTXT(WebSocketsServer &ws){
        /* Send via Websocket */
        String sensordata = "{"
                            "\"name\": \"LM393\","
                            "\"frontleft\": "+String(frontleft_steps)+","
                            "\"frontright\": "+String(frontright_steps)+","
                            "\"backleft\": "+String(backleft_steps)+","
                            "\"backright\": "+String(backright_steps)+","
                            "}";
        ws.broadcastTXT(sensordata);
}