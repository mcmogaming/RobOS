#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "env.h"
#include "constants.h"
#include "mpuSensor.h"
#include "ultrasonicSensor.h"
#include "encoderSensor.h"
#include "motorController.h"
#include "commandProcess.h"

void parseCommand(const char* s);
void clearCommandBuffer();

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

WebSocketsServer webSocket = WebSocketsServer(80);

int counter = 0;
char cmd[MAX_NUM_CMDS][MAX_CMD_LENGTH] = {0};

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length){

  switch(type){

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
    break;

    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connection from ", num);
      Serial.println(ip.toString());
    }
    break;

    case WStype_TEXT:
      Serial.printf("[%u] len:%d Text: %s\n", num, length, payload);

      if(length > 0){
        parseCommand((const char*) payload);
        for(int i = 0; i < MAX_NUM_CMDS; i++){
          Serial.printf("command %d %s\n", i, cmd[i]);
        }
        processCommand(cmd);
        clearCommandBuffer();
      }

      webSocket.sendTXT(num, payload);
    break;

    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;    
  }

}

void setup() {
  Serial.begin(115200);

  Serial.println("Connecting...");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }


  Serial.println("Connected!");
  Serial.print("My IP Address: ");
  Serial.print(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  gyro_setup();
  ultrasonic_setup();
  encoder_setup();
  motorController_setup();

}

void loop() {
  webSocket.loop();
  gyro_loop();
  ultrasonic_loop();
  encoder_loop();
  motorController_loop();
  if(counter % 10 == 0){
  gyroBroadcastClientsTXT(webSocket);
  ultrasonicBroadcastClientsTXT(webSocket);
  encoderBroadcastClientsTXT(webSocket);
  }
  counter++;
}

void parseCommand(const char* s){
  Serial.printf("Text: %s\n", s);
    int i = 0;
    int idx_seperator = 0;
    int idx_count = 0;
    while(s[i] != '\0'){
      if(s[i] == ','){
        strncpy(cmd[idx_count],(const char*)(&s[idx_seperator]), i-idx_seperator);
        idx_seperator = i+1;
        idx_count++;
      }
      i++;
    } 
    Serial.printf("sep: %d count: %d i:%d \n", idx_seperator, idx_count, i);
}

void clearCommandBuffer(){
  for(int i = 0; i < MAX_NUM_CMDS*MAX_CMD_LENGTH; i++){
      cmd[0][i] = 0;
  }
}