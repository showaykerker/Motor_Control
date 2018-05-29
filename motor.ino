#include "Arduino.h"
#include "LTimer.h" //https://goo.gl/caMMxq

#define pinL1 4
#define pinL2 5
#define pinEINT1 3
#define pinEINT2 6

const int interval = 200;
unsigned int n = 0, target=18;
unsigned int now_power = 0;
float error=0, gamma_=0.6, last_error=0;
LTimer timer0(LTIMER_0);


void setup() {
  Serial.begin(9600);
  pinMode(pinL1, OUTPUT);
  pinMode(pinL2, OUTPUT);
  attachInterrupt(pinEINT1, _callback_EINT, CHANGE);
  attachInterrupt(pinEINT2, _callback_EINT2, RISING);
  timer0.begin();
  timer0.start(interval, LTIMER_REPEAT_MODE, _callback_Timer0, NULL);
}


void loop() {
  forward(now_power, 100);
}


/**************** PID Controller     ****************/

void pid(int target_, int now_n){
  float kp=1.1, kd=0.0, ki=0.0;
  error = gamma_*error + (target_-now_n);
  now_power = now_power + int((target_-now_n)*kp) + int(ki*error) + int( (error-last_error)*kd );
  last_error=error;
  Serial.print(now_n);
  Serial.print(",");
  Serial.println(target_);
  //Serial.print(",");
  //Serial.print((target_-now_n)*kp/3);
  //Serial.print(",");
  //Serial.println(now_power/30);
  if (now_power>255) now_power=255;
  if (now_power<0) now_power=0;
}


/**************** CallBack Functions ****************/

void forward(int power, int delay_time) {
  analogWrite(pinL1, power);
  analogWrite(pinL2, 0);
  delay(delay_time);
}


void _callback_EINT(void) {
  n++;
}


void _callback_Timer0(void *usr_data){
  pid(target, n);
  n=0;
}


void _callback_EINT2(void){
  if (target==18) target=36;
  else if (target==36) target=27; 
  else if (target==27) target=15;
  else if (target==15) target=30;
  else if (target==30) target=39;
  else if (target==39) target=21;
  else if (target==21) target=9;
  else if (target==9) target=18;
  else target=18;
}

