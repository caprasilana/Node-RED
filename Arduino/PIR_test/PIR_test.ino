/*
 Controlling a Pir Sensor with Arduino

 by Alberto Celano
 http://www.caprasilana.it
*/

#define PIN_PIR 12 // Out of PIR is connected with pin 12
#define PIN_LED 13 // built-in led

boolean motionDetected = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);

  // reset PIR 
  digitalWrite(PIN_PIR, LOW);
  
  Serial.println("PIR active");
  delay(100);
}

void loop() {

  // IF PIN_PIR is High => motion detected by sensor
  if (digitalRead(PIN_PIR) == HIGH) {
    digitalWrite(PIN_LED, HIGH);   //Turnon Led
    if (!motionDetected) {
      motionDetected = true;
      Serial.println("---");
      Serial.println("Motion Detected!");
      delay(50);
    }
  }
  // IF PIN_PIR is Low => no motion detected by sensor
  if (digitalRead(PIN_PIR) == LOW) {
    digitalWrite(PIN_LED, LOW);  //Turn off Led
   if (motionDetected) {
      motionDetected = false;
      Serial.println("---");
      Serial.println("No motion detected");   
      delay(50);
    }
  }
}
