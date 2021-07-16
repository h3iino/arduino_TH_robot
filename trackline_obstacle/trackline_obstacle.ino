#include <VarSpeedServo.h>

int ENA = 5;
int ENB = 6;
int IN1 = 7;
int IN2 = 8;
int IN3 = 12;
int IN4 = 13;

int Infrared_left = A2;             //Define the infrared interface on the left
int Infrared_right = A3;            //Define the right infrared interface
int IR_L;                           //Infrared sign on the left
int IR_R;                           //Infrared sign on the right
int Left_Speed_Hold = 120;           //Define motor speed variables
int Right_Speed_Hold = 120;          //Define motor speed variables
int movenum = 1;                    //Define the movement(0:stop, 1:go, 2:left, 3:right)
int movenum2 = 1;
int check_grip = 0;                      //Define already grip or not(0:no-grip, 1:grip)

int Echo = A5;  //Define the ultrasonic signal receiving pin
int Trig = A4;  //Define the ultrasonic signal transmission pin
float distance;
float research_distance = 30.0;

/*arm initial value*/
VarSpeedServo servo1, servo2, servo3, servo4;
float th1=180;
float th2=180;
float th3=0;
float th4=90;
int pos = 0;  //This variable is used to store the angle variable
float L1=100,L2=160;
float x,y;
int dep = -90;
int arm=0;

#define MOTOR_GO_FORWARD{digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}
#define MOTOR_GO_BACK{digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}
#define MOTOR_GO_RIGHT{digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}
#define MOTOR_GO_LEFT{digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}
#define MOTOR_GO_STOP{digitalWrite(IN1,LOW);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,LOW);} 


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

  pinMode(Echo,INPUT);
  pinMode(Trig,OUTPUT);

  servo1.attach(11);  //Define the control port of servo 1 as 11
  servo2.attach(2);  //Define the control port of servo 2 as 2
  servo3.attach(4);  //Define the control port of servo 3 as 4
  servo4.attach(3);  //Define the control port of servo 4 as 3
}

void loop() {
  ik(150,150);
  IR_L = digitalRead(A14);
  IR_R = digitalRead(A13);
  research_distance = Get_Distance()*10;
  movenum = trackline(IR_L,IR_R);
  if(check_grip==0){
    if(research_distance < 100){
      MOTOR_GO_STOP;
      arm=0;
      distance = Get_Distance()*10;
      delay(500);
      
      /*open gripper*/
      if(arm==0){
        for(pos = 90; pos >= 25; pos--){
          servo4.write(pos);
          delay(50);
          }
        }
      
      /*Inverse Kinematics*/
      ik(distance+20,dep+30);
      ik(distance+30,dep-5);
    
      /*close gripper*/
      for(pos = 25; pos < 90; pos++){
        servo4.write(pos);
        delay(50);
        }
    
      /*lift up*/
      arm=1;
      if(arm==1){
        ik(100,100);
        arm = 10;
        }
      delay(2500);
      MOTOR_GO_LEFT;
      delay(1400);
      MOTOR_GO_STOP;
      /*lift down*/
      arm=12;
      if(arm==12){
        ik(150,dep);
        open_grip();
        arm = 100;
        } 
      if(arm == 100){
        ik(100,100);
        }     
      MOTOR_GO_RIGHT;
      delay(1200);
      arm = 0;
      MOTOR_GO_STOP;
      delay(100);
      }
    }
  /*move to another place*/
  if(arm == 10){
    if(check_grip == 0){
      MOTOR_GO_FORWARD;
      delay(400);
      check_grip = 1;
      }
    if(check_grip == 1){
      IR_L = digitalRead(A14);
      IR_R = digitalRead(A13);
      movenum2 = trackline(IR_L,IR_R);
    
      if(movenum2 == 0){
        arm=11;
        delay(500);
        /*lift down*/
        arm=12;
        if(arm==12){
          ik(150,dep);
          open_grip();
          arm = 100;
          check_grip = 3;
          }
        }
      if(arm == 100){
        ik(100,100);
        }
      }
    }
    if(check_grip == 3){
      MOTOR_GO_BACK;
      delay(2000);
      MOTOR_GO_STOP;
      delay(3000);
      }
}

int trackline(int IR_L, int IR_R){
  if ((IR_L == HIGH) && (IR_R == HIGH)){
    MOTOR_GO_STOP;//停止
    return 0;
    }
  if ((IR_L == HIGH) && (IR_R == LOW)){
    MOTOR_GO_LEFT;//左转
    return 2;
    }
  if ((IR_L == LOW) && ( IR_R == HIGH)){
    MOTOR_GO_RIGHT;//右转
    return 3;
    }
  if ((IR_L == LOW) && (IR_R == LOW)){
    MOTOR_GO_FORWARD;//直行
    return 1;
    }
  }

void open_grip(){
  for(pos = 90; pos >= 25; pos--){
    servo4.write(pos);
    delay(50);
    }
  }
  
void ik(float x,float y){
  th1=acos((x*x+y*y+L1*L1-L2*L2)/(2*L1*sqrt(x*x+y*y)))+atan2(y,x);
  th2=atan2(y-L1*sin(th1),x-L1*cos(th1))-th1;
  th1=th1*180/3.14 + 40;
  th2=th2*180/3.14;
  if(th1<0){
    th1 += 360;
    }
  if(th2<0){
    th2 = 180+th2;
    }
  set_servo();
}

 void set_servo(){
    servo1.write(th1,30,true);
    servo2.write(th2,30,true);
//    servo3.write(th3,30,true);
//    servo4.write(th4,30,true);
}

float Get_Distance(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);

  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  float Ldistance = pulseIn(Echo,HIGH,5000);
  Ldistance = Ldistance / 5.8 / 10;
  if(Ldistance < 3){
    Ldistance = 20;
    }
  return Ldistance;
  }

float Get_X(float d){
  float alpha = 3.14/2 - atan2(12,d) - 3.14*5/18;
  float x = sqrt(d*d + 12*12) * cos(alpha);
  return x;
  }
float Get_Y(float d){
  float alpha = 3.14/2 - atan2(12,d) - 3.14*5/18;
  float y = sqrt(d*d + 12*12) * sin(alpha);
  return y;
  }
