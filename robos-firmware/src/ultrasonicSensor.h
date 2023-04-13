#include <WebSocketsServer.h>
#include <Arduino.h>

//Functions
void ultrasonic_setup();
void ultrasonic_loop();
float getDistanceCm();
void ultrasonicBroadcastClientsTXT(WebSocketsServer &ws);