#include <Servo.h>

// ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

// led pins
const int greenLED = 7;
const int yellowLED = 8;
const int redLED = 13;

// buzzer and servo pins
const int buzzerPin = 3;
const int servoPin = 5;

// servo object
Servo servo;

// distance thresholds in cm!!
const int closeThreshold = 60;      // red zone
const int mediumThreshold = 90;     // yellow zone
const int farThreshold = 122;       // green zone

// servo sweep variables
int servoAngle = 0;
int servoDirection = 1;             // 1 for increasing, -1 for decreasing

// timing
unsigned long lastMeasureTime = 0;
unsigned long lastServoTime = 0;

void setup() {
  // pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // servo setup
  servo.attach(servoPin);
  servo.write(90);                  // start at middle

}

void loop() {
  unsigned long currentTime = millis();
  
  // measure distance every 100ms
  if (currentTime - lastMeasureTime > 100) {
    lastMeasureTime = currentTime;
    
    // get distance
    long distance = getDistance();
    
    // turn off all leds and buzzer first
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzerPin, LOW);
    
    // check distance and control leds
    if (distance < closeThreshold) {
      // close - red and buzzer
      digitalWrite(redLED, HIGH);
      digitalWrite(buzzerPin, HIGH);
    } else if (distance < mediumThreshold) {
      // medium - yellow
      digitalWrite(yellowLED, HIGH);
    } else if (distance < farThreshold) {
      // far - green
      digitalWrite(greenLED, HIGH);
    }
  }
  
  // sweep servo every 50ms
  if (currentTime - lastServoTime > 50) {
    lastServoTime = currentTime;
    
    // move servo
    servoAngle += servoDirection * 2;
    
    // reverse direction at limits
    if (servoAngle >= 180) {
      servoAngle = 180;
      servoDirection = -1;
    } else if (servoAngle <= 0) {
      servoAngle = 0;
      servoDirection = 1;
    }
    
    servo.write(servoAngle);
  }
}

// measure
long getDistance() {
  // send pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // measure echo time
  long duration = pulseIn(echoPin, HIGH);
  
  // convert to cm (duration in microseconds / 58)
  long distance = duration / 58;
  
  return distance;
}
