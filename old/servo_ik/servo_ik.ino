//#include<Servo.h>
#include <VarSpeedServo.h>

//Servo servo1, servo2, servo3, servo4;
VarSpeedServo servo1, servo2, servo3, servo4;

float th1=90;
float th2=90;
float th3=0;
float th4=90;
//float phi,ld,l;
int pos = 0;  //This variable is used to store the angle variable
 
float L1=100,L2=160;
 
float x,y;
int arm=0;

void setup() {
  Serial.begin(9600);
  servo1.attach(11);  //Define the control port of servo 1 as 11
  servo2.attach(2);  //Define the control port of servo 1 as 11
  servo3.attach(4);  //Define the control port of servo 1 as 11
  servo4.attach(3);  //Define the control port of servo 1 as 11
}

void loop() {
//  for(pos = -90; pos < -30; pos++){
//    Serial.println(pos);
//    servo3.write(pos);
//    delay(50);
//    }
//  for(pos = -30; pos >= -90; pos--){
//    servo3.write(pos);
//    delay(50);
//    }
//  Serial.println(pos);

  arm=0;
  delay(500);
  /*open gripper*/
  for(pos = 90; pos >= 25; pos--){
    servo4.write(pos);
    delay(50);
    }
  
  /*Inverse Kinematics*/
  ik(150,-20);

  /*close gripper*/
  for(pos = 25; pos < 90; pos++){
    servo4.write(pos);
    delay(50);
    }

  /*lift up*/
  arm=1;
  if(arm==1){
    ik(100,100);
    }
  delay(5000);

  /*lift down*/
  arm=2;
  if(arm==2){
    ik(150,-20);
    for(pos = 90; pos >= 25; pos--){
      servo4.write(pos);
      delay(50);
      }
    }
}

void ik(float x,float y){
  th1=acos((x*x+y*y+L1*L1-L2*L2)/(2*L1*sqrt(x*x+y*y)))+atan2(y,x);
  th2=atan2(y-L1*sin(th1),x-L1*cos(th1))-th1;
  th1=th1*180/3.14;
  th2=th2*180/3.14;
  if(th1<0){
    th1 += 360;
    }
  if(th2<0){
    th2 = 180+th2+15;
    }
  set_servo();
}


 void set_servo(){
    servo1.write(th1,30,true);
    servo2.write(th2,30,true);
//    servo3.write(th3,30,true);
//    servo4.write(th4,30,true);
}
