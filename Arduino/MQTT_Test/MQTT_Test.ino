/*
 Test MQTT publish and subscribe via etherner connection

 by Alberto Celano
 http://www.caprasilana.it
*/
#include <Ethernet.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER "192.168.1.5"
#define AIO_SERVERPORT 1883 // use 8883 for SSL
#define AIO_USERNAME ""
#define AIO_KEY ""


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

     /* if (strcmp((char *)subscribeTopic.lastread, "ON_MESSAGE") == 0)
      {
         Serial.println((char *)subscribeTopic.lastread);
      }*/
      Serial.print(F("Got: "));
      Serial.println((char *)subscribeTopic.lastread);
     
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
