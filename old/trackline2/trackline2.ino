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
//  digitalWrite(ENB,HIGH);
//  digitalWrite(ENA,HIGH);
}

void loop() {
//  IR_L = digitalRead(A2);
//  IR_R = digitalRead(A3);
  IR_L = digitalRead(A14);
  IR_R = digitalRead(A13);

  Serial.println(IR_L);
  
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
