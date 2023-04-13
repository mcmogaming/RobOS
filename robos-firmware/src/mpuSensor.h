#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WebSocketsServer.h>

//Functions
void gyro_setup();
void gyro_loop();
sensors_event_t get_data_bytes();

//WS
void gyroBroadcastClientsBIN(WebSocketsServer &ws);
void gyroBroadcastClientsTXT(WebSocketsServer &ws);
