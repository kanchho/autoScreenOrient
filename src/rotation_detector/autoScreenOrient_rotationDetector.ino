/*
Authors : Bibek Koirala, Rodrigo Baranda
Name : autoScreenOrient
Description : A device that detects rotational motion and transmits it over bluetooth
*/


/*
* MPU-6050 communicates with Arduino via I2C serial communication(by default).

* HC-05 module is a Bluetooth SPP (Serial Port Protocol) module, so it also communicates with Arduino via Serial Communication. 
  Instead of built-in UART hardware serial, this project uses Software Serial library.
  
* All non-standard libraries used can be found in rotation_detector/lib/. Copy them 
  to C:/Program Files(x86)/Arduino/libraries or similiar path on your PC.

* Calibration of MPU-6050 is done by setting offsets.
  - Manual calibration : https://www.i2cdevlib.com/forums/topic/91-how-to-decide-gyro-and-accelerometer-offsett/ 
  - Calibration can also be done by using the tool found in rotation_detector/tools/
  - Calibration by tool explained :  https://www.youtube.com/watch?v=z9G7UloEG9k
*/


#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <SoftwareSerial.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define SWITCH_ANGLE 45       // trigger angle for switching orientation from landscape to potrait and vice versa
#define LEVEL_ANGLE 0


MPU6050 mpu;
bool dmpReady = false;           // If DMP init is succesful, dmpReady = true
uint8_t mpuInterruptStatus;      
uint8_t devStatus;               // device status (0 = success)
uint16_t packetSize;             // DMP packet size
uint16_t fifoCount;             // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; 

Quaternion quaternion;                   // [w, x, y, z]
VectorFloat gravity;                     // [x, y, z]
float ypr[3];                            // [yaw, pitch, roll]
volatile bool mpuInterrupt = false;      // Is set to true when gyro data is ready and  MPU interrupts uC.

SoftwareSerial bluetooth(11, 12);        // RX, TX

byte lastAngleState = 0;

void setup()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24;                                             // 400kHz I2C clock
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(115200);
  bluetooth.begin(9600);
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  
  mpu.setXGyroOffset(200);  // More accurate calibration will be done after soldering pin header to MPU-6050 
  mpu.setYGyroOffset(70);
  mpu.setZGyroOffset(-75);
  mpu.setZAccelOffset(1788); 
  if (devStatus == 0) {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuInterruptStatus = mpu.getIntStatus();
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void loop()
{
  if (!dmpReady) return;
  mpuInterrupt = false;
  mpuInterruptStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuInterruptStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
  } else if (mpuInterruptStatus & 0x02) {
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&quaternion, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &quaternion);
    mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
    int x = ypr[0] * 180 / M_PI;
    int y = ypr[1] * 180 / M_PI;
    int z = ypr[2] * 180 / M_PI;
    /*
    z-axis has been used below for detecting rotation. Out of 3 axes, only one is used for detecting 
    rotation and it depends on how MPU-6050 is placed on breadboard and how breadboard is mounted
    on display. This axis can be found by printing values of all axes in serial monitor and looking
    for  the axis whose value changes when screen is rotated.
    */
    if (isAngleChanged(z)) {
      transmitNewRotation(z);
    }
  }
}

boolean isAngleChanged(int angle) 
{
  byte newAngleState = getAngleState(angle);
  if (lastAngleState != newAngleState) {
    lastAngleState = newAngleState;
    return true;
  }

  return false;
}

byte getAngleState(int angle) 
{
  byte state;
  if (abs(angle - LEVEL_ANGLE) < SWITCH_ANGLE) {
    state = 0;
  } else if (angle - LEVEL_ANGLE > 0) {
    state = 1;
  } else {
    state = 2;
  }

  return state;
}

void transmitNewRotation(int angle) 
{
  Serial.println(getAngleState(angle));
  bluetooth.print(getAngleState(angle));
}

void dmpDataReady() 
{
  mpuInterrupt = true;
}
