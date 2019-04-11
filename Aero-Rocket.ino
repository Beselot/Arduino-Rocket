#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

Servo topLeft;
Servo topRight;
Servo backLeft;
Servo backRight;
Servo parachute;
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);

int topLeftServoID = 7;
int topRightServoID = 8;
int backLeftServoID = 9;
int backRightServoID = 10;
int parachuteServoID = 11;

int initialPosition = 90;
int upperCorrectionPosition = 120;
int lowerCorrectionPosition = 60;

int parachuteIN = 0;
int parachuteOUT = 90;

int xAccel = 0;
int yAccel = 0;
int zAccel = 0;

int xGyro = 0;
int yGyro = 0;
int zGyro = 0;

int interval = 2000;
int accelThreshold = 2;
int xUpperThreshold = 2;
int xLowerThreshold = 2;
int yUpperThreshold = 2;
int yLowerThreshold = 2;

void setupSensor()
{
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);

  topLeft.attach(topLeftServoID);
  topRight.attach(topRightServoID);
  backLeft.attach(backLeftServoID);
  backRight.attach(backRightServoID);
  parachute.attach(parachuteServoID);

  topLeft.write(initialPosition);
  topRight.write(initialPosition);
  backLeft.write(initialPosition);
  backRight.write(initialPosition);
  parachute.write(parachuteIN);

  lsm.begin();

  delay(200);
}

void loop() {

  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  xAccel = a.acceleration.x;
  yAccel = a.acceleration.y;
  zAccel = a.acceleration.z;

  xGyro = g.gyro.x;
  yGyro = g.gyro.y;
  zGyro = g.gyro.z;

  if (millis() > interval)
  {
    if (zAccel < accelThreshold)
    {
      topLeft.write(initialPosition);
      topRight.write(initialPosition);
      backLeft.write(initialPosition);
      backRight.write(initialPosition);
      parachute.write(parachuteOUT);
    }
  }

  if (xAccel > xUpperThreshold)
  {
    topLeft.write(upperCorrectionPosition);
    topRight.write(upperCorrectionPosition);
  }
  else if (xAccel < xLowerThreshold)
  {
    topLeft.write(lowerCorrectionPosition);
    topRight.write(lowerCorrectionPosition);
  }
  else if (xAccel < xUpperThreshold && xAccel > xLowerThreshold)
  {
    topLeft.write(initialPosition);
    topRight.write(initialPosition);
  }




  if (yAccel > yUpperThreshold)
  {
    backLeft.write(upperCorrectionPosition);
    backRight.write(upperCorrectionPosition);
  }
  else if (yAccel < yLowerThreshold)
  {
    backLeft.write(lowerCorrectionPosition);
    backRight.write(lowerCorrectionPosition);
  }
  else if (yAccel < yUpperThreshold && yAccel > yLowerThreshold)
  {
    backLeft.write(initialPosition);
    backRight.write(initialPosition);
  }

}
