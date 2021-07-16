#include <Servo.h>

Servo servo1;

int pos = 90;  //This variable is used to store the angle variable

void setup() {
  servo1.attach(11);  //Define the control port of servo 1 as 11
}

void loop() {
  for(pos = 90; pos < 171; pos++){
    servo1.write(pos);
    delay(50);
    }
  for(pos = 170; pos >= 90; pos--){
    servo1.write(pos);
    delay(50);
    }
}
