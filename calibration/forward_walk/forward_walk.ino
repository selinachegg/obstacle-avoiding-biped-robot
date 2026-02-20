/*
 * Forward Walk — Calibration Sketch
 *
 * Walks the robot forward continuously without obstacle detection.
 * Use this to tune servo angles for a balanced forward gait.
 *
 * Board:   Arduino UNO
 * Wiring:  RightUp=pin4, LeftUp=pin5, RightLow=pin6, LeftLow=pin7
 * Library: Servo.h (built-in)
 */

#include <Servo.h>

Servo monServo_Rightup;
Servo monServo_Leftup;
Servo monServo_Rightlow;
Servo monServo_Leftlow;

void setup() {
  monServo_Rightup.attach(4);
  monServo_Leftup.attach(5);
  monServo_Rightlow.attach(6);
  monServo_Leftlow.attach(7);

  // Neutral standing position
  monServo_Leftlow.write(60);
  monServo_Leftup.write(95);
  monServo_Rightup.write(85);
  monServo_Rightlow.write(130);
  delay(500);

  Serial.begin(9600);
  Serial.println("Forward walk calibration started");
}

void loop() {
  // Right leg step
  monServo_Rightlow.write(140);   // lower leg down
  delay(100);
  monServo_Rightup.write(70);    // upper leg forward
  delay(100);
  monServo_Rightlow.write(110);  // lower leg up
  delay(100);
  monServo_Rightup.write(105);   // upper leg back
  delay(100);

  // Left leg step
  monServo_Leftlow.write(45);    // lower leg down
  delay(100);
  monServo_Leftup.write(105);   // upper leg forward
  delay(100);
  monServo_Leftlow.write(80);   // lower leg up
  delay(100);
  monServo_Leftup.write(70);    // upper leg back
  delay(100);
}
