import logo from './logo.svg';
import './App.css';
import React, {useEffect, useState} from 'react';
import Switch from '@mui/material/Switch';
import OrientationVisualizerComponent from './components/orientationvisualizer/OrientationVisualizerComponent';
import {RecoilRoot, useRecoilState} from 'recoil';
import {mpu6050} from './components/sensors/GlobalSensorVariables';
import Gamepad from 'react-gamepad';

function App() {

  const [socket, setSocket] = useState([]);
  const [wsAddress, setWsAddress] = useState('ws://192.168.2.35:80');
  const [statusMessage, setStatusMessage] = useState(''); 
  const [consoleTextStream, setConsoleTextStream] = useState([]);
  const [wsMesssage, setWSMessage] = useState('');
  const [allowmultiline, setMultLineConsole] = useState(true);
  const [sensorMPU6050, setMPU6050] = useRecoilState(mpu6050);
  const [gyroOffset, setGyroOffset] = useState({x:0,y:0,z:0});
  const [robotMotorPower, setRobotMotorPower] = useState({left:0,right:0});
  const [xboxController, setXboxController] = useState({'LeftStick':{x:0,y:0}});

  function handleChangeWSAddress(e){
    setWsAddress(e.target.value);
  }

  function handleButtonConnect(e){
    Promise.resolve().then(()=>{
      const s = new WebSocket(wsAddress);
      setSocketEventListeners(s);
      return s;
    })
    .then(s => setSocket(s));
  }

  function setSocketEventListeners(s){
    console.log("Event Listener Added");

    s.onmessage = function (event){
      console.log("Message from server", event.data);
      if(event.data[0] == "{"){
      if(allowmultiline){
        consoleTextStream.push(event.data);
        setConsoleTextStream(consoleTextStream.slice());
      }else{
        setConsoleTextStream([event.data]);
      }
    }
    };  

    socket.onopen = (event) => {
    };

    socket.onerror = (event) => {
      console.log("WebSocket error: ", event);
    };

    socket.onclose = (event) => {
      console.log("The connection has been closed successfully.");
    };
  }

  function ConsoleTextViewer(props){
    let consoleText = [];
    for(let line in consoleTextStream){
      consoleText.push(consoleTextStream[line]);
      consoleText.push(<br/>);
    }

    return <div>{consoleText}</div>;
  }



  function handleChangeWSMessage(e){
    setWSMessage(e.target.value);
  }

  function handleSendWSMessage(){
    socket.send(wsMesssage);
  }

  const label = { inputProps: { 'aria-label': 'Multi Line Console' } };

  function handleMultiLineSwitch(e){
    setMultLineConsole(e.target.checked);
  }

  useEffect(()=>{UpdateSensorValues()}, [consoleTextStream]);

  function UpdateSensorValues(){
    //"{\"name\": \"MPU6050\",\"gyro\": {\"x\": -0.03,\"y\": 0.04,\"z\": -0.02},\"acceleration\": {\"x\": 0.08,\"y\": -2.19,\"z\": 9.62},\"temperature\": {\"value\": 27.52}}"
    //console.log("consoletextstream", consoleTextStream);
    try{
    const sensor = JSON.parse(consoleTextStream[0]);
    //console.log("sensor json", sensor)
    switch(sensor.name){
      case 'MPU6050':
          //Add Gyro Offset
          sensor.gyro.x -= gyroOffset.x;
          sensor.gyro.y -= gyroOffset.y;
          sensor.gyro.z -= gyroOffset.z;

          setMPU6050({...sensor});
          //console.log(sensor);
        break;
      default:
        break;
    }
  }catch(e){}
  }

  function handleCalibrateGyro(){
    setGyroOffset({x:sensorMPU6050.gyro.x, y:sensorMPU6050.gyro.y, z:sensorMPU6050.gyro.z});
  }

  function controllerAxisChangeHandler(axisName, value, previousValue){
    switch(axisName){
      case "LeftStickX":
        xboxController.LeftStick.x = value;
        setXboxController({...xboxController});
      break;
      case "LeftStickY":
        xboxController.LeftStick.y = value;
        setXboxController({...xboxController});
      break;
      default:
      break;
    }
    axisChangeHandler(axisName,value,previousValue);
  }

  useEffect(()=>{robotMoveCommand();}, [xboxController]);

  function robotMoveCommand(){
    robotMotorPower.left = Math.round(255*xboxController.LeftStick.y);
    robotMotorPower.right = Math.round(255*xboxController.LeftStick.y);
    robotMotorPower.left += Math.round(255*xboxController.LeftStick.x);
    robotMotorPower.right -= Math.round(255*xboxController.LeftStick.x);

    setRobotMotorPower({...robotMotorPower});

    try{
    //if(new Date().getMilliseconds() % 2 == 0 || (robotMotorPower.left == robotMotorPower.right == 0)){
    socket.send(`motor,${robotMotorPower.left},${robotMotorPower.right},`);
    //}  
  }catch(e){}
  }

  function connectHandler(gamepadIndex) {
    console.log(`Gamepad ${gamepadIndex} connected !`);
  }
 
  function disconnectHandler(gamepadIndex) {
    console.log(`Gamepad ${gamepadIndex} disconnected !`);
  }
 
  function buttonChangeHandler(buttonName, down) {
    console.log(buttonName, down);
  }

  function axisChangeHandler(axisName, value, previousValue) {
    console.log(axisName, value);
  }
 
  function buttonDownHandler(buttonName) {
    console.log(buttonName, 'down');
  }
 
  function buttonUpHandler(buttonName) {
    console.log(buttonName, 'up');
  }

  return (
    <div className="App">
        <h1>RobOS Dashboard</h1>
        <input type="text" name="websocket-address" onChange={handleChangeWSAddress} defaultValue={"ws://192.168.2.35:80"}/>
        <button onClick={handleButtonConnect}>Connect</button>
        <h2>{statusMessage}</h2>
        <ConsoleTextViewer/>
        <input type="text" name="websocket-message" onChange={handleChangeWSMessage}  defaultValue={" "}/>
        <button onClick={handleSendWSMessage}>Send Message</button>
        <Switch onChange={handleMultiLineSwitch} {...label} defaultChecked />
        <OrientationVisualizerComponent/>
        <button onClick={handleCalibrateGyro}>Calibrate</button>
        <Gamepad onConnect={connectHandler} onDisconnect={disconnectHandler}
        onButtonChange={buttonChangeHandler} onAxisChange={controllerAxisChangeHandler}>
          <p>Controller</p>
        </Gamepad>
    </div>
  );
}


export default App;
