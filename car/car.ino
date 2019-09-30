#include <Servo.h>

Servo angleServo;
int servoPin = 2;

Servo leftServo;
Servo rightServo;
int rightSpeed = 0;
int leftSpeed = 0;
int choTri[3] = {3,4,5};
int choEcho[3] = {6,7,8};
long distance[3];

int basicDis = 60;
int goBasicDis = 20;
int angle=90;
void maneuver(int speedLeft, int speedRight);

unsigned int prev_time1 = 0;
unsigned int curr_time1 = 0;

void setup() {
  
  for(int i = 0; i < 3; i++)
  {
    pinMode(choTri[i], OUTPUT);
    pinMode(choEcho[i], INPUT);
  }
  angleServo.attach(servoPin);
  leftServo.attach(12);
  rightServo.attach(13);
  leftServo.writeMicroseconds(1496);
  rightServo.writeMicroseconds(1494);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(choTri[i], LOW);
    delayMicroseconds(2);
    digitalWrite(choTri[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(choTri, LOW);
    distance[i] = (pulseIn(choEcho[i], HIGH, 170003 )*340)/2/10000;
    if(distance[i] == 0)
      distance[i] = 500;
  }
  if(distance[0] >= 200 || distance[1] >= 200 || distance[2] >= 200)
    return ;
  int gway = way();
  if(distance[0] < basicDis && distance[2] < basicDis)
  {
      if(gway == 0)
      {
        angle += 5;
      }
      else
      {
        angle -= 5;
      }
  }
  else if(distance[0] < basicDis && gway == 0)
  {
    angle += 5;
  }
  else if(distance[2] < basicDis && gway == 2)
  {
    angle -= 5;
  }
  
  if(angle < 20)
    angle = 20;
  else if(angle > 160)
    angle = 160;
    
  angleServo.write(angle);

  curr_time1 = millis();
  if(curr_time1 - prev_time1 > 200)
  {
    if(distance[1] <= goBasicDis || distance[0] < goBasicDis || distance[2] < goBasicDis || distance[0] > basicDis && distance[1] > basicDis && distance[2] > basicDis)
      maneuver(0,0);
    else if(angle >= 125 && ((distance[0] >= goBasicDis && distance[0] < basicDis) || (distance[1] >= goBasicDis && distance[1] < basicDis) || (distance[2] >= goBasicDis && distance[2] < basicDis)))
      maneuver(-50, 50);
    else if(angle >= 100 && ((distance[0] >= goBasicDis && distance[0] < basicDis) || (distance[1] >= goBasicDis && distance[1] < basicDis) || (distance[2] >= goBasicDis && distance[2] < basicDis)))
      maneuver(50, 200);
    else if(angle <= 55 && ((distance[0] >= goBasicDis && distance[0] < basicDis) || (distance[1] >= goBasicDis && distance[1] < basicDis) || (distance[2] >= goBasicDis && distance[2] < basicDis)))
      maneuver(50, -50);
    else if(angle <= 80 && ((distance[0] >= goBasicDis && distance[0] < basicDis) || (distance[1] >= goBasicDis && distance[1] < basicDis) || (distance[2] >= goBasicDis && distance[2] < basicDis)))
      maneuver(200, 50);
    else
      maneuver(200, 200);
  }
  Serial.print(distance[0]);
  Serial.print(" ");
  Serial.print(distance[1]);
  Serial.print(" ");
  Serial.println(distance[2]);
  delay(50);
}

void maneuver(int speedLeft, int speedRight)
{
  if(rightSpeed != speedRight && leftSpeed != speedLeft)
  {
    leftServo.writeMicroseconds(1496+speedLeft);// Set Left servo speed
    rightServo.writeMicroseconds(1494 - speedRight);
    Serial.print(speedLeft);
    Serial.print(" ");
    Serial.println(speedRight);
    prev_time1 = millis();
    if(speedLeft == speedRight != 0)
      prev_time1 = 0;
    rightSpeed = speedRight;
    leftSpeed = speedLeft;
  }
}

int way()
{
  int result = 0;
  if(distance[0] < distance[1] && distance[0] < distance[2])
  {
    result = 0;
  }
  else if(distance[1] < distance[0] && distance[1] < distance[2])
  {
    result = 1;
  }
  else if(distance[2] < distance[0] && distance[2] < distance[1])
  {
    result = 2;
  }
  return result;
}
