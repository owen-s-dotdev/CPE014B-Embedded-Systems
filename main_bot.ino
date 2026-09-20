#include <NewPing.h>

#define TRIG_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 200
#define MAX_SPEED 190
#define MAX_SPEED_OFFSET 20

int IRSensorRight = A0;
int IRSensorLeft = A1;

NewPing SONAR(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int distance;
int MOTORLEFT[2] = {2,3};
int MOTORRIGHT[2] = {4,5};

void setup() {
  Serial.begin(9600);

  pinMode(MOTORLEFT[0], OUTPUT);
  pinMode(MOTORRIGHT[0], OUTPUT);
  
  digitalWrite(MOTORLEFT[0], LOW);
  digitalWrite(MOTORRIGHT[0], LOW);
  analogWrite(MOTORLEFT[1], 0);
  analogWrite(MOTORRIGHT[1], 0);
  
pinMode(IRSensorRight, INPUT_PULLUP);
  pinMode(IRSensorLeft, INPUT_PULLUP);
  delay(1000);
  for(int i = 0; i < 4; i++){ //get initial distance upon power up
    GET_DISTANCE();
    delay(100);
  }
  moveStop();
  moveForward();
}

void loop() {
  GET_DISTANCE();
  if(distance < 10 && distance > 0){
    PUSH();
    delay(300);
    moveStop();
    delay(50);
//    for(int i = 0; i < 4; i++){ //get initial distance upon power up
//    GET_DISTANCE();
//    delay(100);
//  }
  }

  if(digitalRead(IRSensorRight)){
      moveStop();
      moveBackward();
      delay(300);
      turnLeft();
      delay(180);
      //moveStop();
      moveForward();
      //delay(50);
      
  }if(digitalRead(IRSensorLeft)){
      moveStop();
      moveBackward();
      delay(300);
      turnRight();
      delay(180);
      //moveStop();
      moveForward();
      //delay(50);
  }else{
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
  //Serial.println("PUSH!!!!");
  digitalWrite(MOTORRIGHT[0], HIGH);
  digitalWrite(MOTORLEFT[0], HIGH);
  analogWrite(MOTORRIGHT[1], 0);
  analogWrite(MOTORLEFT[1], 0);
  //delay(2000);
}

void moveForward(){
  //Serial.println("forward");
  digitalWrite(MOTORRIGHT[0], HIGH);
  digitalWrite(MOTORLEFT[0], HIGH);
  analogWrite(MOTORRIGHT[1], 165);
  analogWrite(MOTORLEFT[1], 180);
}

void moveBackward(){
  //Serial.println("back");
  digitalWrite(MOTORRIGHT[0], LOW);
  digitalWrite(MOTORLEFT[0], LOW);
  analogWrite(MOTORRIGHT[1], 180);
  analogWrite(MOTORLEFT[1], 180);
}

void turnLeft(){
  //Serial.println("left");
  digitalWrite(MOTORRIGHT[0], HIGH);
  digitalWrite(MOTORLEFT[0], LOW);
  analogWrite(MOTORRIGHT[1], 0);
  analogWrite(MOTORLEFT[1], 200);
}

void turnRight(){
  //Serial.println("right");
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
