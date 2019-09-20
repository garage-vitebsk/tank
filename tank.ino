#include <Arduino.h>
#include "L298N.h"

DriveSystem *driveSystem = new DriveSystem(new Motor(6, 7, 8),
    new Motor(11, 10, 9));

void setup() {
  Serial.begin(19200);
}

void loop() {
  int speedLeft = Serial.parseInt();
  int speedRight = Serial.parseInt();
  if (speedLeft != 0 || speedRight != 0){
    driveSystem->setSpeed(speedLeft, speedRight);
  }
  delay(10);
}
