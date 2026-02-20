/*
 * IR Sensor Test — Calibration Sketch
 *
 * Reads distance from a Sharp GP2Y0A21YK0F IR sensor and prints
 * the value to the Serial Monitor. Use this to verify sensor wiring
 * and calibrate the obstacle detection threshold.
 *
 * Board:   Arduino UNO
 * Wiring:  IR sensor signal → A0
 * Library: SharpIR.h (install via Library Manager)
 *
 * Expected output: distance in cm (valid range 10–80 cm)
 */

#include <SharpIR.h>

#define IR_PIN A0
#define MODEL  1080  // GP2Y0A21YK0F

int distance_cm;

SharpIR mySensor = SharpIR(IR_PIN, MODEL);

void setup() {
  Serial.begin(9600);
  Serial.println("Sharp IR sensor test — GP2Y0A21YK0F");
  Serial.println("Place objects at known distances to verify readings.");
}

void loop() {
  distance_cm = mySensor.distance();

  Serial.print("Mean distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(1000);
}
