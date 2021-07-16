//#include <stdlib.h>

int Echo = A5;  //Define the ultrasonic signal receiving pin
int Trig = A4;  //Define the ultrasonic signal transmission pin

void setup() {
  Serial.begin(9600);
  pinMode(Echo,INPUT);
  pinMode(Trig,OUTPUT);
  delay(2000);
}

void loop() {
  Serial.println(Get_Distance());

}

float Get_Distance(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);

  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  float Ldistance = pulseIn(Echo,HIGH,5000);
  Ldistance = Ldistance / 5.8 / 10;
  return Ldistance;
  }
