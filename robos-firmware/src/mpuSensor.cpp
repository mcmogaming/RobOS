#include "mpuSensor.h"
#include <stdio.h>

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
sensors_event_t sensordata[3];

// int accelrange = MPU6050_RANGE_8_G;
// int gyrorange = MPU6050_RANGE_500_DEG;
// int filterbandwidth = MPU6050_BAND_21_HZ;

void gyro_setup(){
    Serial.println("Initializing MPU 6050 Sensor");

    if (!mpu.begin()){
        Serial.println("Failed to find MPU6050");
        while(1){
            delay(10);
        }
    }

    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}


void gyro_loop(){
    mpu.getEvent(&a, &g, &temp);
      /* Print out the values */
        // Serial.print("Acceleration X: ");
        // Serial.print(a.acceleration.x);
        // Serial.print(", Y: ");
        // Serial.print(a.acceleration.y);
        // Serial.print(", Z: ");
        // Serial.print(a.acceleration.z);
        // Serial.println(" m/s^2");

        // Serial.print("Rotation X: ");
        // Serial.print(g.gyro.x);
        // Serial.print(", Y: ");
        // Serial.print(g.gyro.y);
        // Serial.print(", Z: ");
        // Serial.print(g.gyro.z);
        // Serial.println(" rad/s");

        // Serial.print("Temperature: ");
        // Serial.print(temp.temperature);
        // Serial.println(" degC");

        // Serial.println("");
}

void gyroBroadcastClientsBIN(WebSocketsServer &ws){
    memcpy(&sensordata, &a, sizeof(sensors_event_t));
    memcpy(&sensordata[1*sizeof(sensors_event_t)], &g, sizeof(sensors_event_t));
    memcpy(&sensordata[2*sizeof(sensors_event_t)], &temp, sizeof(sensors_event_t));

    ws.broadcastBIN((uint8_t*)&sensordata, 3*sizeof(sensors_event_t));

}

void gyroBroadcastClientsTXT(WebSocketsServer &ws){
        /* Send via Websocket */
        String sensordata = "{"
                            "\"name\": \"MPU6050\","
                            "\"gyro\": {"
                                "\"x\": "+String(g.gyro.x)+","
                                "\"y\": "+String(g.gyro.y)+","
                                "\"z\": "+String(g.gyro.z)+""
                            "},"
                            "\"acceleration\": {"
                                "\"x\": "+String(a.acceleration.x)+","
                                "\"y\": "+String(a.acceleration.y)+","
                                "\"z\": "+String(a.acceleration.z)+""
                            "},"
                            "\"temperature\": {"
                                "\"value\": "+String(temp.temperature)+""
                            "}"
                            "}";
        ws.broadcastTXT(sensordata);
}

sensors_event_t get_data_bytes(){
    return a, g, temp;
}

