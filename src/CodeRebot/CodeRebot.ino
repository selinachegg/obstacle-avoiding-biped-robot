/*
 * Obstacle-Avoiding Biped Robot — Main Controller
 *
 * A bipedal walking robot that navigates autonomously using two Sharp IR
 * distance sensors and four servo motors (two per leg: upper + lower joint).
 * When an obstacle is detected, the robot turns away or reverses.
 *
 * Board:    Arduino UNO
 * Shield:   Sensor Shield v5.0 (EasyCard)
 * Authors:  CHEGGOUR Selina, GOUFFI Djamel, MBOCK Therese
 * Advisor:  RIBERO Jean-Marc
 * Date:     2020/2021 — Master 1 ESTEL
 *
 * Libraries:
 *   - Servo.h   (built-in)
 *   - SharpIR.h (install via Library Manager)
 */

#include <Servo.h>
#include <SharpIR.h>

// ── IR Sensor Configuration ──────────────────────────────────────────────────
#define IR_PIN_RIGHT       A0
#define IR_PIN_LEFT        A1
#define IR_MODEL           1080    // Sharp GP2Y0A21YK0F (10–80 cm)
#define OBSTACLE_THRESHOLD 20     // cm — obstacle detected below this distance

// ── Servo Pin Assignments ────────────────────────────────────────────────────
#define SERVO_RIGHT_UP_PIN  4
#define SERVO_LEFT_UP_PIN   5
#define SERVO_RIGHT_LOW_PIN 6
#define SERVO_LEFT_LOW_PIN  7

// ── Indicator Pin Assignments ────────────────────────────────────────────────
#define LED_RIGHT_PIN 12
#define LED_LEFT_PIN  13
#define BUZZER_PIN    8

// ── Servo Neutral (Standing) Positions ───────────────────────────────────────
#define NEUTRAL_RIGHT_LOW 130
#define NEUTRAL_RIGHT_UP   85
#define NEUTRAL_LEFT_UP    95
#define NEUTRAL_LEFT_LOW   60

// ── Forward Gait — Right Leg ─────────────────────────────────────────────────
#define FWD_R_LOW_DOWN  140
#define FWD_R_UP_FRONT   70
#define FWD_R_LOW_UP    110
#define FWD_R_UP_BACK   105

// ── Forward Gait — Left Leg ──────────────────────────────────────────────────
#define FWD_L_LOW_DOWN   45
#define FWD_L_UP_FRONT  105
#define FWD_L_LOW_UP     80
#define FWD_L_UP_BACK    70

// ── Reverse Gait — Left Leg ─────────────────────────────────────────────────
#define REV_L_UP_BACK    40
#define REV_L_LOW_UP     80
#define REV_L_UP_FRONT   90
#define REV_L_LOW_DOWN   45

// ── Reverse Gait — Right Leg ────────────────────────────────────────────────
#define REV_R_UP_BACK   120
#define REV_R_LOW_UP    110
#define REV_R_UP_FRONT   90
#define REV_R_LOW_DOWN  140

// ── Turn Right — Right Leg Only ──────────────────────────────────────────────
#define TURN_R_UP_BACK  145
#define TURN_R_LOW_UP   110
#define TURN_R_UP_FRONT 110
#define TURN_R_LOW_DOWN 140

// ── Turn Left — Left Leg Only ────────────────────────────────────────────────
#define TURN_L_UP_BACK   40
#define TURN_L_LOW_UP    80
#define TURN_L_UP_FRONT  65
#define TURN_L_LOW_DOWN  45

// ── Timing ──────────────────────────────────────────────────────────────────
#define STEP_DELAY       100  // ms between servo movements during walking
#define REVERSE_DELAY     60  // ms between servo movements during reverse
#define REVERSE_DELAY_R   50  // ms for right leg during reverse
#define BUZZER_PULSE      50  // ms buzzer on-time for obstacle alert
#define REVERSE_STEPS      4  // number of backward steps when both sensors blocked
#define TURN_STEPS         2  // number of turning steps

// ── Global Objects ──────────────────────────────────────────────────────────
Servo servoRightUp;
Servo servoLeftUp;
Servo servoRightLow;
Servo servoLeftLow;

SharpIR sensorRight = SharpIR(IR_PIN_RIGHT, IR_MODEL);
SharpIR sensorLeft  = SharpIR(IR_PIN_LEFT,  IR_MODEL);

int distanceRight;
int distanceLeft;

// ── Helper: buzzer alert ────────────────────────────────────────────────────
void buzzerAlert() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(BUZZER_PULSE);
  digitalWrite(BUZZER_PIN, LOW);
}

// ── Helper: return to neutral stance ────────────────────────────────────────
void standNeutral() {
  servoLeftLow.write(NEUTRAL_LEFT_LOW);
  servoLeftUp.write(NEUTRAL_LEFT_UP);
  servoRightUp.write(NEUTRAL_RIGHT_UP);
  servoRightLow.write(NEUTRAL_RIGHT_LOW);
}

// ── Setup ───────────────────────────────────────────────────────────────────
void setup() {
  servoRightUp.attach(SERVO_RIGHT_UP_PIN);
  servoLeftUp.attach(SERVO_LEFT_UP_PIN);
  servoRightLow.attach(SERVO_RIGHT_LOW_PIN);
  servoLeftLow.attach(SERVO_LEFT_LOW_PIN);

  standNeutral();
  delay(500);

  pinMode(LED_RIGHT_PIN, OUTPUT);
  pinMode(LED_LEFT_PIN,  OUTPUT);
  pinMode(BUZZER_PIN,    OUTPUT);

  digitalWrite(LED_RIGHT_PIN, HIGH);
  digitalWrite(LED_LEFT_PIN,  HIGH);
  digitalWrite(BUZZER_PIN,    LOW);
}

// ── Main Loop ───────────────────────────────────────────────────────────────
void loop() {
  distanceRight = sensorRight.distance();
  distanceLeft  = sensorLeft.distance();

  bool obstacleRight = (distanceRight <= OBSTACLE_THRESHOLD && distanceRight >= 0);
  bool obstacleLeft  = (distanceLeft  <= OBSTACLE_THRESHOLD && distanceLeft  >= 0);

  // ── Both sensors blocked → reverse, then turn toward clearer side ─────
  if (obstacleRight && obstacleLeft) {
    buzzerAlert();
    standNeutral();

    // Walk backward
    for (int i = 0; i < REVERSE_STEPS; i++) {
      servoLeftUp.write(REV_L_UP_BACK);
      delay(REVERSE_DELAY);
      servoLeftLow.write(REV_L_LOW_UP);
      delay(REVERSE_DELAY);
      servoLeftUp.write(REV_L_UP_FRONT);
      delay(REVERSE_DELAY);
      servoLeftLow.write(REV_L_LOW_DOWN);
      delay(REVERSE_DELAY);

      servoRightUp.write(REV_R_UP_BACK);
      delay(REVERSE_DELAY_R);
      servoRightLow.write(REV_R_LOW_UP);
      delay(REVERSE_DELAY_R);
      servoRightUp.write(REV_R_UP_FRONT);
      delay(REVERSE_DELAY_R);
      servoRightLow.write(REV_R_LOW_DOWN);
      delay(REVERSE_DELAY_R);
    }
    delay(500);

    // Re-read sensors and turn toward the side with more clearance
    distanceRight = sensorRight.distance();
    distanceLeft  = sensorLeft.distance();

    if (distanceRight > distanceLeft) {
      // Turn right (step with right leg only)
      for (int i = 0; i < TURN_STEPS; i++) {
        servoRightUp.write(TURN_R_UP_BACK);
        delay(STEP_DELAY);
        servoRightLow.write(TURN_R_LOW_UP);
        delay(STEP_DELAY);
        servoRightUp.write(TURN_R_UP_FRONT);
        delay(STEP_DELAY);
        servoRightLow.write(TURN_R_LOW_DOWN);
        delay(STEP_DELAY);
      }
    } else {
      // Turn left (step with left leg only)
      for (int i = 0; i < TURN_STEPS; i++) {
        servoLeftUp.write(TURN_L_UP_BACK);
        delay(STEP_DELAY);
        servoLeftLow.write(TURN_L_LOW_UP);
        delay(STEP_DELAY);
        servoLeftUp.write(TURN_L_UP_FRONT);
        delay(STEP_DELAY);
        servoLeftLow.write(TURN_L_LOW_DOWN);
        delay(STEP_DELAY);
      }
    }
  }
  // ── Right sensor blocked → turn left (step left leg only) ─────────────
  else if (obstacleRight) {
    buzzerAlert();
    standNeutral();

    for (int i = 0; i < TURN_STEPS; i++) {
      servoRightLow.write(FWD_R_LOW_DOWN);
      delay(STEP_DELAY);
      servoRightUp.write(FWD_R_UP_FRONT + 10);  // 80° — slightly reduced stride
      delay(STEP_DELAY);
      servoRightLow.write(FWD_R_LOW_UP);
      delay(STEP_DELAY);
      servoRightUp.write(FWD_R_UP_BACK);
      delay(STEP_DELAY);
      servoRightLow.write(NEUTRAL_RIGHT_LOW);
      delay(STEP_DELAY);
    }
  }
  // ── Left sensor blocked → turn right (step right leg only) ────────────
  else if (obstacleLeft) {
    buzzerAlert();
    standNeutral();

    for (int i = 0; i < TURN_STEPS; i++) {
      servoLeftLow.write(FWD_L_LOW_DOWN);
      delay(STEP_DELAY);
      servoLeftUp.write(FWD_L_UP_FRONT);
      delay(STEP_DELAY);
      servoLeftLow.write(FWD_L_LOW_UP);
      delay(STEP_DELAY);
      servoLeftUp.write(FWD_L_UP_BACK);
      delay(STEP_DELAY);
      servoLeftLow.write(NEUTRAL_LEFT_LOW);
      delay(STEP_DELAY);
    }
  }
  // ── No obstacle → walk forward ────────────────────────────────────────
  else {
    // Right leg step
    servoRightLow.write(FWD_R_LOW_DOWN);
    delay(STEP_DELAY);
    servoRightUp.write(FWD_R_UP_FRONT);
    delay(STEP_DELAY);
    servoRightLow.write(FWD_R_LOW_UP);
    delay(STEP_DELAY);
    servoRightUp.write(FWD_R_UP_BACK);
    delay(STEP_DELAY);

    // Left leg step
    servoLeftLow.write(FWD_L_LOW_DOWN);
    delay(STEP_DELAY);
    servoLeftUp.write(FWD_L_UP_FRONT);
    delay(STEP_DELAY);
    servoLeftLow.write(FWD_L_LOW_UP);
    delay(STEP_DELAY);
    servoLeftUp.write(FWD_L_UP_BACK);
    delay(STEP_DELAY);
  }
}
