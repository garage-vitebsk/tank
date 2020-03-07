#include <Arduino.h>
#include "L298N.h"
#include <iarduino_RF433_Receiver.h>

#define DEBUG true
#define STOP_DELAY 1000

iarduino_RF433_Receiver radio(2);

byte data[3] = {0, 0, 0};
long receiveTime;

int speedLeft = 0;
int speedRight = 0;
int speedTower = 0;

MotorDriver *towerDriver = new MotorDriver(new Motor(5, 3, 4));

DriveSystem *driveSystem = new DriveSystem(new Motor(6, 7, 8),
    new Motor(11, 10, 9));

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  radio.begin();
  radio.setDataRate(i433_1KBPS);
  radio.openReadingPipe(1);
  radio.startListening();
}

void loop() {
  long currentTime = millis();
  if (radio.available()) {
    radio.read(&data, sizeof(data));
    speedLeft = map(data[0], 0, 256, -255, 255);
    speedRight = map(data[1], 0, 256, -255, 255);
    speedTower = map(data[2], 0, 256, -255, 255);
    digitalWrite(13, HIGH);
    receiveTime = currentTime;
  } else {
    if (currentTime - receiveTime > STOP_DELAY) {
      speedLeft = 0;
      speedRight = 0;
      speedTower = 0;
    }
    digitalWrite(13, LOW);
  }
#if DEBUG
  Serial.print("Speed: ");
  Serial.print(speedLeft);
  Serial.print(" ");
  Serial.print(speedRight);
  Serial.print(" ");
  Serial.print(speedTower);
  Serial.println();
#endif
  driveSystem->setSpeed(speedLeft, speedRight);
  towerDriver->setSpeed(speedTower);
  delay(10);
}
