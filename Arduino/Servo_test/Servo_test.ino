/*
 Controlling a servomotor with Arduino

 by Alberto Celano
 http://www.caprasilana.it
*/

#include <Servo.h>

#define PIN_SERVO 9 // Arduino PIN9

// create servo object to control a servo
Servo servo;  


int val;    // variable to read the value from the analog pin

void setup() {
  // attaches the servo on pin 9 to the servo object
  servo.attach(PIN_SERVO);  
}

void loop() {
  // move to pos 0
  servo.write(0); 
  // wait 1 sec
  delay(1000); 
  // move to pos 45
  servo.write(45); 
  // wait 1 sec
  delay(1000);
  // move to pos 90
  servo.write(90); 
  // wait 1 sec
  delay(1000);
  // move to pos 180
  servo.write(180); 
  // wait 1 sec
  delay(1000);                         
}
