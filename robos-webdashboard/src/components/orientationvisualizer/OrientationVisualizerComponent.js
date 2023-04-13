import * as THREE from 'three';

import { createRoot } from 'react-dom/client'
import React, { useRef, useState } from 'react'
import { Canvas, useFrame } from '@react-three/fiber'
import {useRecoilState} from 'recoil';
import { mpu6050 } from '../sensors/GlobalSensorVariables';

function Box(props) {
  // This reference will give us direct access to the mesh
  const mesh = useRef()
  // Set up state for the hovered and active state
  const [hovered, setHover] = useState(false)
  const [active, setActive] = useState(false)
  // Subscribe this component to the render-loop, rotate the mesh every frame
  const [sensorMPU6050, setMPU6050] = useRecoilState(mpu6050);

  function updateMeshRotation(){
        mesh.current.rotation.x += 0.1*sensorMPU6050.gyro.x;
        mesh.current.rotation.y += 0.1*sensorMPU6050.gyro.y;
        mesh.current.rotation.z += 0.1*sensorMPU6050.gyro.z;
  }

  useFrame((state, delta) => 
    {
        
        if(sensorMPU6050.name == "MPU6050"){
            updateMeshRotation();
        }
    });
  // Return view, these are regular three.js elements expressed in JSX
  return (
    <mesh
      {...props}
      ref={mesh}
      scale={active ? 3 : 2}
      onClick={(event) => setActive(!active)}
      onPointerOver={(event) => setHover(true)}
      onPointerOut={(event) => setHover(false)}>
      <boxGeometry args={[1, 1, 1]} />
      <meshStandardMaterial color={hovered ? 'hotpink' : 'orange'} />
    </mesh>
  )
}

function OrientationVisualizerComponent(){
   return(
    <Canvas>
        <ambientLight />
        <pointLight position={[10, 10, 10]} />
        <Box position={[0, 0, 0]} />
    </Canvas>
  );
}

export default OrientationVisualizerComponent;