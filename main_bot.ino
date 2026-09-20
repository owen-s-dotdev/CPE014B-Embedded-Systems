#include <NewPing.h>

#define TRIG_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 200
#define MAX_SPEED 190
#define MAX_SPEED_OFFSET 20

// Front IR Sensors
int IRSensorRight = A0;
int IRSensorLeft = A1;
// Back IR Sensors (Assign to available analog/digital pins)
int IRSensorBackRight = A2; 
int IRSensorBackLeft = A3;  

NewPing SONAR(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int distance;
int MOTORLEFT[2] = {2,3};
int MOTORRIGHT[2] = {4,5};
int on_button = 6; // pin 6 push button

void setup() {
  Serial.begin(9600);

  pinMode(on_button, INPUT_PULLUP); // ON_OFF push button 

  pinMode(MOTORLEFT[0], OUTPUT);
  pinMode(MOTORRIGHT[0], OUTPUT);
  
  digitalWrite(MOTORLEFT[0], LOW);
  digitalWrite(MOTORRIGHT[0], LOW);

  analogWrite(MOTORLEFT[1], 0);
  analogWrite(MOTORRIGHT[1], 0);
  
  pinMode(IRSensorRight, INPUT_PULLUP);
  pinMode(IRSensorLeft, INPUT_PULLUP);
  pinMode(IRSensorBackRight, INPUT_PULLUP); // Initialize Back Right
  pinMode(IRSensorBackLeft, INPUT_PULLUP);  // Initialize Back Left
  delay(1000);

  for(int i = 0; i < 4; i++){ //get initial distance upon power up
    GET_DISTANCE();
    delay(100);
  }
  
  moveStop();

  while(digitalRead(on_button) == HIGH){
    delay(100);
  }

  moveForward();
}

void loop() {

  GET_DISTANCE();
  
  // 1. Attack Logic (Highest Priority)
  if(distance < 10 && distance > 0){
    PUSH();
    delay(300);
    moveStop();
    delay(50);
  }

  // 2. Edge Detection Logic (Properly chained with else-if)
  if (digitalRead(IRSensorRight)) {
      // Front right hits the line: backup and turn left
      moveStop();
      moveBackward();
      delay(300);
      turnLeft();
      delay(180);
      moveForward();
      
  } else if (digitalRead(IRSensorLeft)) {
      // Front left hits the line: backup and turn right
      moveStop();
      moveBackward();
      delay(300);
      turnRight();
      delay(180);
      moveForward();
      
  } else if (digitalRead(IRSensorBackRight)) {
      // Back right hits the line: drive forward and turn left
      moveStop();
      moveForward();
      delay(300);
      turnLeft();
      delay(180);
      moveForward();
      
  } else if (digitalRead(IRSensorBackLeft)) {
      // Back left hits the line: drive forward and turn right
      moveStop();
      moveForward();
      delay(300);
      turnRight();
      delay(180);
      moveForward();
      
  } else {
      // 3. Default state if no line is detected and no enemy is close
      moveForward();
  }
}

void moveStop(){
  Serial.println("stop");
  for(int i = 0; i < 2; i++){
    digitalWrite(MOTORRIGHT[i], LOW);
    digitalWrite(MOTORLEFT[i], LOW);
  }
}

void PUSH(){
  digitalWrite(MOTORRIGHT[0], HIGH);
  digitalWrite(MOTORLEFT[0], HIGH);
  analogWrite(MOTORRIGHT[1], 0);
  analogWrite(MOTORLEFT[1], 0);
}

void moveForward(){
  digitalWrite(MOTORRIGHT[0], HIGH);
  digitalWrite(MOTORLEFT[0], HIGH);
  analogWrite(MOTORRIGHT[1], 165);
  analogWrite(MOTORLEFT[1], 180);
}

void moveBackward(){
  digitalWrite(MOTORRIGHT[0], LOW);
  digitalWrite(MOTORLEFT[0], LOW);
  analogWrite(MOTORRIGHT[1], 180);
  analogWrite(MOTORLEFT[1], 180);
}

void turnLeft(){
  digitalWrite(MOTORRIGHT[0], HIGH);
  digitalWrite(MOTORLEFT[0], LOW);
  analogWrite(MOTORRIGHT[1], 0);
  analogWrite(MOTORLEFT[1], 200);
}

void turnRight(){
  digitalWrite(MOTORRIGHT[0], LOW);
  digitalWrite(MOTORLEFT[0], HIGH);
  analogWrite(MOTORRIGHT[1], 200);
  analogWrite(MOTORLEFT[1], 0);
}

void GET_DISTANCE(){
  delay(20);
  unsigned int uS = SONAR.ping();
  distance = uS/US_ROUNDTRIP_CM;
  Serial.print("distance: "), Serial.println(distance);
}