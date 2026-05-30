#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <FluxGarage_RoboEyes.h>

#define I2C_SDA 8
#define I2C_SCL 9

#define i2c_Address 0x3c

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

RoboEyes<Adafruit_SH1106G> roboEyes(display);
Adafruit_MPU6050 mpu;

// ===================== STATE =====================

enum RobotState {
  IDLE,
  SHAKING,
  DIZZY,
  ANGRY_STATE,
  FLAT_EYES,
  SAD_STATE
};

RobotState currentState = IDLE;

unsigned long stateStartTime = 0;
unsigned long lastShakeTime = 0;
unsigned long idleEventTimer = 0;

bool idleLaughPlayed = false;
bool idleHappyPlayed = false;

// ===================== SHAKE CONFIG =====================

float shakeThreshold = 4.0; 
unsigned long shakeStopDelay = 700;

// ===================== HELPER =====================

void changeState(RobotState newState) {
  currentState = newState;
  stateStartTime = millis();

  switch (newState) {
    case IDLE:
      roboEyes.setHFlicker(OFF, 0);
      roboEyes.setVFlicker(OFF, 0);
      roboEyes.setSweat(OFF);

      roboEyes.setWidth(36, 36);
      roboEyes.setHeight(36, 36);
      roboEyes.setBorderradius(8, 8);
      roboEyes.setSpacebetween(10);

      roboEyes.setMood(DEFAULT);
      roboEyes.setAutoblinker(ON, 3, 2);
      roboEyes.setIdleMode(ON, 2, 2);

      idleEventTimer = millis();
      idleLaughPlayed = false;
      idleHappyPlayed = false;
      break;

    case SHAKING:
      roboEyes.setIdleMode(OFF, 0, 0);
      roboEyes.setAutoblinker(OFF, 0, 0);
      roboEyes.setMood(DEFAULT);
      roboEyes.setHFlicker(ON, 5);
      roboEyes.setVFlicker(ON, 3);
      break;

    case DIZZY:
      roboEyes.setHFlicker(OFF, 0);
      roboEyes.setVFlicker(OFF, 0);
      roboEyes.setMood(TIRED);
      roboEyes.anim_confused();
      break;

    case ANGRY_STATE:
      roboEyes.setMood(ANGRY);
      roboEyes.setHFlicker(ON, 2);
      roboEyes.setVFlicker(OFF, 0);
      break;

    case FLAT_EYES:
      roboEyes.setHFlicker(OFF, 0);
      roboEyes.setMood(DEFAULT);
      roboEyes.setHeight(8, 8);
      roboEyes.setBorderradius(2, 2);
      roboEyes.setSpacebetween(12);
      break;

    case SAD_STATE:
      roboEyes.setHeight(20, 20);
      roboEyes.setBorderradius(6, 6);
      roboEyes.setMood(TIRED);
      roboEyes.setSweat(ON);
      break;
  }
}

bool detectShake() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

  float accelMag = sqrt(ax * ax + ay * ay + az * az);

  float shakeStrength = abs(accelMag - 9.81);

  if (shakeStrength > shakeThreshold) {
    lastShakeTime = millis();
    return true;
  }

  return false;
}

// ===================== SETUP =====================

void setup() {
  Serial.begin(9600);

  Wire.begin(I2C_SDA, I2C_SCL);
  delay(250);

  display.begin(i2c_Address, true);

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);

  if (!mpu.begin(0x68, &Wire)) {
    Serial.println("MPU6050 tidak terdeteksi!");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  changeState(IDLE);
}

// ===================== LOOP =====================

void loop() {
  roboEyes.update();

  bool shakeDetected = detectShake();

  switch (currentState) {
    case IDLE:
      if (shakeDetected) {
        changeState(SHAKING);
      }

      if (millis() - idleEventTimer > 3000 && !idleHappyPlayed) {
        idleHappyPlayed = true;
        roboEyes.setMood(HAPPY);
      }

      if (millis() - idleEventTimer > 5000 && !idleLaughPlayed) {
        idleLaughPlayed = true;
        roboEyes.anim_laugh();
      }

      if (millis() - idleEventTimer > 8000) {
        roboEyes.setMood(DEFAULT);
        idleEventTimer = millis();
        idleHappyPlayed = false;
        idleLaughPlayed = false;
      }
      break;

    case SHAKING:
      if (millis() - lastShakeTime > shakeStopDelay) {
        changeState(DIZZY);
      }
      break;

    case DIZZY:
      if (millis() - stateStartTime > 1800) {
        changeState(ANGRY_STATE);
      }
      break;

    case ANGRY_STATE:
      if (millis() - stateStartTime > 1800) {
        changeState(FLAT_EYES);
      }
      break;

    case FLAT_EYES:
      if (millis() - stateStartTime > 1500) {
        changeState(SAD_STATE);
      }
      break;

    case SAD_STATE:
      if (millis() - stateStartTime > 2500) {
        changeState(IDLE);
      }
      break;
  }
}