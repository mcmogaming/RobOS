#include <Arduino.h>
#include <WebSocketsServer.h>

void encoder_setup();
void encoder_loop();
void encoderBroadcastClientsTXT(WebSocketsServer &ws);