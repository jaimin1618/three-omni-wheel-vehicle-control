
# Three omni wheel vehicle control

A 3-omni wheel vehicle equipped with a PID control system for precise directional navigation, controllable through both a PS3 controller and the MIT Android app.
## Features

- Movement in 2D - XY plane. 
- Vehicle control option using PS3. 
- Vehicle Control option using MIT android app. 
- Auto correcting path and direction errors - inbuilt PID control system using MPU6050. 

## Tech Stack

**Software:** MIT app inventor - application  
**Microcontrollers:** Arduino Mega 2560, ESP8266/ESP32, MPU6050  
**Control:** MIT App, PS3  

## Videos

#### mobile control
![Vehicle control using android-app](omni-wheel-with-mobile-app.mp4)

#### PS3 control
![Vehicle control using PS3](omni-wheel-with-ps3.mp4)



## Setup & Installation

Clone the project

```bash
  git clone https://github.com/jaimin1618/three-omni-wheel-vehicle-control.git
```

Go to the project directory

```bash
  cd three-omni-wheel-vehicle-control
```


#### Upload programs in microcontrollers

**MPU6050 calibration program file:** mpu6050-calibration-program/mpu6050-calibration-program.ino  
**ESP8266 program file:** esp8266-program/esp8266-program.ino  
**Arduino Mega 2560 program file:** three-omni-wheel-vehicle-control.ino

## License

[MIT](https://choosealicense.com/licenses/mit/)


## Authors

- [Jaimin Chokhawala](https://github.com/jaimin1618)

