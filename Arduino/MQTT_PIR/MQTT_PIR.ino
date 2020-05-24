/*
 Test MQTT publish and subscribe via etherner connection 
 PIR Sensor for motion detect

 by Alberto Celano
 http://www.caprasilana.it
*/
#include <Ethernet.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER "192.168.1.5" // IP of MQTT BROKER
#define AIO_SERVERPORT 1883 // use 8883 for SSL
#define AIO_USERNAME ""
#define AIO_KEY ""

/************************* Define PIR PIN *********************************/

#define PIN_PIR 12 // Out of PIR is connected with pin 12
#define PIN_LED 13 // built-in led

boolean motionDetected = false;

/************************* Network Properties *********************************/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };; // Arduino MAC
IPAddress ip(192, 168, 1, 35); // Arduino IP
IPAddress server(192, 168, 1, 5); // MQTT Broker IP

/************************* Ethernet Client Setup *****************************/

EthernetClient ethClient;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&ethClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'publishTopic' for publishing.
Adafruit_MQTT_Publish publishTopic = Adafruit_MQTT_Publish(&mqtt, "publishTopic");

// Setup a feed called 'subscribeTopic' for subscribing to changes.
Adafruit_MQTT_Subscribe subscribeTopic = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "subscribeTopic");

void MQTT_connect();

void setup()
{  
  Serial.begin(9600);

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);

  // reset PIR 
  digitalWrite(PIN_PIR, LOW);
  delay(10);
  Serial.println();
  Serial.print("Connecting to network");
  
  Ethernet.begin(mac, ip);
  Serial.println();
  
  // Setup MQTT subscription
  mqtt.subscribe(&subscribeTopic);
}

void loop()
{
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &subscribeTopic)
    {
      Serial.print(F("Got: "));
      Serial.println((char *)subscribeTopic.lastread);     
    }
  }

  // IF PIN_PIR is High => motion detected by sensor
  if (digitalRead(PIN_PIR) == HIGH) {
    digitalWrite(PIN_LED, HIGH);   //Turnon Led
    if (!motionDetected) {
      motionDetected = true;
      Serial.println("---");
      Serial.println("Motion Detected!");
      // send message via MQTT
      
      publishTopic.publish("MOTION_START"); 
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

      // send message via MQTT
      publishTopic.publish("MOTION_END"); 
      delay(50);
    }
  }
  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
  
  publishTopic.publish("Hello MQTT World!"); 
}
