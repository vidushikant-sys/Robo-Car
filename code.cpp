
#include <Servo.h>

// L298N
#define ENA 5
#define IN1 8
#define IN2 9
#define ENB 6
#define IN3 10
#define IN4 11

// Sensors
#define TOUCH_SENSOR 2
#define IR_SENSOR 3

// Ultrasonic
#define TRIG 12
#define ECHO 13

// Servo
#define SERVO_PIN 4

Servo myservo;

const int SPEED = 150;
const int OBSTACLE_DISTANCE = 15;

int servoAngle = 90;
int servoStep = 2;

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TOUCH_SENSOR, INPUT);
  pinMode(IR_SENSOR, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(90);

  Serial.begin(9600);
}

void loop() {

  // Servo continuous scan
  servoAngle += servoStep;

  if (servoAngle >= 150) servoStep = -2;
  if (servoAngle <= 30) servoStep = 2;

  myservo.write(servoAngle);

  // IR edge detection
  if (digitalRead(IR_SENSOR) == HIGH) {

    stopRobot();
    delay(100);

    backward();
    delay(700);

    turnRight();
    delay(600);

    stopRobot();
    return;
  }

  // Touch sensor
  if (digitalRead(TOUCH_SENSOR) == HIGH) {

    stopRobot();
    delay(100);

    backward();
    delay(700);

    turnLeft();
    delay(600);

    stopRobot();
    return;
  }

  // Ultrasonic obstacle detection
  int distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > OBSTACLE_DISTANCE) {

    forward();

  } else {

    stopRobot();
    delay(100);

    backward();
    delay(400);

    stopRobot();
    delay(100);

    // Check Left
    myservo.write(150);
    delay(400);
    int leftDistance = getDistance();

    // Check Right
    myservo.write(30);
    delay(400);
    int rightDistance = getDistance();

    // Center
    myservo.write(90);
    delay(300);

    if (leftDistance > rightDistance) {

      turnLeft();
      delay(500);

    } else {

      turnRight();
      delay(500);
    }

    stopRobot();
    delay(100);
  }

  delay(20);
}

// Distance Function
int getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0)
    return 250;

  return duration * 0.034 / 2;
}

// Motor Functions
void forward() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
