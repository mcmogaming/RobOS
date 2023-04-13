import React from 'react';
import {
  RecoilRoot,
  atom,
  selector,
  useRecoilState,
  useRecoilValue,
} from 'recoil';

export const mpu6050 = atom({
    key: 'sensor_mpu6050', // unique ID (with respect to other atoms/selectors)
    default: {name:"MPU6050", gyro:{x:0, y:0, z:0}, acceleration:{x:0,y:0,z:0}, temperature:{value:0}}, // default value (aka initial value)
  });

export const hc_sr04 = atom({
    key: 'sensor_mpu6050', // unique ID (with respect to other atoms/selectors)
    default: {name:"HC-SR04", distanceCm:3, distanceInch:3}, // default value (aka initial value)
  });