#include <Servo.h>
#include <EEPROM.h>
#include "LCD12864RSPI.h"
#define SIZE( a ) sizeof( a ) / sizeof( a[0] )


int ENA = 5;                        //TB6612 enable A
int ENB = 6;                        //TB6612 enable B
int INPUT1 = 7;                     //Motor interface 1
int INPUT2 = 8;                     //Motor interface 2
int INPUT3 = 12;                    //Motor interface 3
int INPUT4 = 13;                    //Motor interface 4         

int Input_Detect_TrackLeft = A2;     //定义小车跟随左侧红外
int Input_Detect_TrackRight = A3;    //定义小车跟随右侧红外

//int Infrared_left = A2;             //Define the infrared interface on the left
//int Infrared_right = A3;            //Define the right infrared interface
int IR_L;                           //Infrared sign on the left
int IR_R;                           //Infrared sign on the right
int Left_Speed_Hold = 65;           //Define motor speed variables
int Right_Speed_Hold = 65;          //Define motor speed variables


#define MOTOR_GO_FORWARD  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}      //车体前进                              
#define MOTOR_GO_BACK     {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}      //车体后退
#define MOTOR_GO_RIGHT    {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}      //车体右转
#define MOTOR_GO_LEFT     {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}      //车体左转
#define MOTOR_GO_STOP     {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,LOW);}        //车体停止


void setup() {
  Serial.begin(9600);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);  
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  analogWrite(ENB,Left_Speed_Hold);
  analogWrite(ENA,Right_Speed_Hold);
}

void loop() {
  delay(1000);
  TrackLine();

}


void TrackLine()   // 巡线模式
{
  IR_L = digitalRead(Input_Detect_TrackLeft);//读取左边传感器数值
  IR_R = digitalRead(Input_Detect_TrackRight);//读取右边传感器数值
  if ((IR_L == HIGH) && (IR_R == HIGH))//左右都检测到，就如视频中的那样遇到一道横的胶带
      {
        MOTOR_GO_STOP;//停止
      }
      
      if ((IR_L == HIGH) && (IR_R == LOW)) //左侧遇到黑线
      {
        MOTOR_GO_LEFT;//左转
      }
      
      if ((IR_L == LOW) && ( IR_R == HIGH)) //右侧遇到黑线
      {
        MOTOR_GO_RIGHT;//右转
      }
      
      if ((IR_L == LOW) && (IR_R == LOW))//俩边都没有检测到黑线表明在轨迹中
      {
        MOTOR_GO_FORWARD;//直行
      }
  }
