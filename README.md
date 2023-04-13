# RobOS
RobOS is open source firmware for robots on ESP32 platform

Author: Mohammad Rahman

## Inspiration
This project was inspired by the software stack used in the CSCC85 course by Paco Estrada. In that course
we built a 2 wheel lego EV3 robot and used a simple bluetooth api in C to control it and receive sensor data. 
RobOS was built to provide a similar experience for DIY robotics enthusiasts to write more code and worry less about
writing code for retrieving and filtering sensor data, multi threading/interrupts, and motor movement and PID loops.

## Features
* Websocket [5 connections]
    * Read Sensor Values
    * Send Commands
* Xbox Controller Support
* Sensor Data Visualization

## Build

### 1.React
Within the robos-webdashboard folder run

```
npm -i
```

```
npm start
```

### 2.ESP32 Robot
Modify the firmware/env.h file to reflect the sensors and pinout arrangement you have on
your robot. In the env.h you must also configure your WiFi credentials.

Boot up your robot and get the ip from the serial console or network router. Afterwards
type in ws://yourrobotip:80 in the webdashboard and click connect.

Your robot should be connected now! and you should be able to control it with 
an xbox controller and read sensor data.






