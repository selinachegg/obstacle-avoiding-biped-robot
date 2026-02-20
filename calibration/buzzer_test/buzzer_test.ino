/*
 * Buzzer Test — Calibration Sketch
 *
 * Pulses the piezoelectric buzzer to verify wiring and sound output.
 * The buzzer emits a short 50 ms pulse followed by a 950 ms pause.
 *
 * Board:  Arduino UNO
 * Wiring: Buzzer signal → pin 8
 */

#define BUZZER_PIN 8

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Buzzer test — pulsing every second");
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
  delay(950);
}
