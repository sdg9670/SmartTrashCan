#include <Servo.h>

//전면 초음파 센서
int Trig_Pin1 = 2;
int Echo_Pin1 = 3;

//천장 초음파 센서
int Trig_Pin2 = 8;
int Echo_Pin2 = 9;

//RGB LED
int R = 4;
int G = 5;
int B = 6;

//부저
int Buzzor = 7;

//컴퓨터 이전 저장시간, 현재시간
unsigned int prev_time1 = 0;
unsigned int curr_time1 = 0;

//서보모터
Servo servo1;

// 열린, 닫힌 각도
int close_angle = 80;//85
int open_angle = 160;
//모터 각도
int angle = close_angle;

boolean cho = true;
int count = 0;
long duration1, distance1=999; 
long duration2, distance2 = 999 ;

void setup() {
    //시리얼 통신 속도 설정
    Serial.begin(9600);

    //servo1 포트 설정
    servo1.attach(10);

    //전면 초음파 센서 핀모드 설정
    pinMode(Echo_Pin1, INPUT);
    pinMode(Trig_Pin1, OUTPUT);
    
    //천장 초음파 센서 핀모드 설정
    pinMode(Echo_Pin2, INPUT);
    pinMode(Trig_Pin2, OUTPUT);
    
    //부저 핀모드 설정
    pinMode(Buzzor, OUTPUT);

    //RGB LED 핀모드 설정
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);
}

void loop() {
    //현재 시간 저장
    curr_time1 = millis();
    if(cho)
    {
      //duration 초음파 측정 값, distance 측정값을 변환한 거리
      digitalWrite(Trig_Pin1, HIGH);//초음파 출력   
      delayMicroseconds(10);//0.010초 동안 초음파 출력되도록 딜레이
      digitalWrite(Trig_Pin1, LOW);//초음파 미출력
      duration1 = pulseIn(Echo_Pin1, HIGH, 17647);//echo가 HIGH 되기까지 시간
      distance1 = ((long)(340 * duration1) / 10000) / 2; //초음파 속도와 시간을 이용해 거리 환산
      Serial.print("1: ");
      Serial.println(distance1);
      if(distance1 != 0 && distance1 < 500)
      {
        if(distance1 <= 30 && angle == close_angle) // 뚜겅닫힌지 1초 후 거리가 30cm 이하면 뚜껑 열림
        {
          count --;
          if(count < -1)
          {
            prev_time1 = millis();
            angle = open_angle;
            servo1.write(angle);
            Serial.println("열림");
            count = 0;
          }
        }
        else if(curr_time1 -  prev_time1 >= 3000 && distance1 > 30 && angle != close_angle) // 뚜겅열린지 1초 후 거리가 30cm 초과면 뚜껑 닫힘
        {
          count ++;
          if(count > 1)
          {
            angle = close_angle;
            servo1.write(angle);
            Serial.println("닫힘");
            Serial.println(distance1);
            count = 0;
          }
        }
        else
          count = 0;
      }
      Serial.println(count);
    }
    else if(angle == close_angle)
    {
      //duration 초음파 측정 값, distance 측정값을 변환한 거리
      digitalWrite(Trig_Pin2, HIGH);//초음파 출력   
      delayMicroseconds(10);//0.010초 동안 초음파 출력되도록 딜레이
      digitalWrite(Trig_Pin2, LOW);//초음파 미출력
      duration2 = pulseIn(Echo_Pin2, HIGH, 5000);//echo가 HIGH 되기까지 시간
      distance2 = ((long)(340 * duration2) / 10000) / 2; //초음파 속도와 시간을 이용해 거리 환산
      Serial.print("2: ");
      Serial.println(distance2);
      //10cm 이하일 경우 레드 led
      if(distance2 != 0)
      {
        if(distance2 <= 15)
        {
          digitalWrite(R, HIGH);
          digitalWrite(G, LOW);
          digitalWrite(B, LOW);
        }
        else if(distance2 <= 22) //18 이하일 경우 엘로우 led
        {
          digitalWrite(R, HIGH);
          digitalWrite(G, HIGH);
          digitalWrite(B, LOW);
        }
        else // 18 초과는 블루
        {
          digitalWrite(R, LOW);
          digitalWrite(G, LOW);
          digitalWrite(B, HIGH);
        }
  
        //뚜겅이 열린 상태에서 빈공간이 10cm 이하면 부저 울림
        if(angle == close_angle && distance2 <= 15)
          digitalWrite(Buzzor, LOW);
        else // 그외엔 부저 안울림
          digitalWrite(Buzzor, HIGH);
      }
    }
    cho = cho ? false : true;
    delay(50);
}
