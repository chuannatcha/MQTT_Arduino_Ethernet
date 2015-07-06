/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xAC };
//byte server[] = { 198, 41, 30, 241};  //iot.eclipse.org
byte server[] = { 128, 199, 104, 122};

char* outTopic = "chuan_arduino_eth_out";
char* inTopic = "chuan_arduino_eth_in";

String data = "data:";
String data2 = data + " test2";

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
   byte* p = (byte*)malloc(length);
  memcpy(p,payload,length);
  char* text = (char*)p;
  strlcpy(text,text,length+1);
  client.publish(outTopic,text);
  Serial.println(text);
  if(text == "on")
    Serial.println("Power On");
  else if(text == "off")
    Serial.println("Power Off");
  else
    Serial.println("Unknow Command");
  free(p);
}

void setup()
{ Serial.begin(115200);
  Ethernet.begin(mac);
  if (client.connect("chuan_arduino_eth")) {
    Serial.println("MQTT Connected");
    client.publish(outTopic,"hello world");
    client.subscribe(inTopic);
  }
}

void loop()
{
  client.loop();
  if((millis()%2500)==0)
  { 
    String data_send = data + " millis=" + String(millis()) + " adc=" + String(analogRead(A0));
    if(client.publish(outTopic,(char*)data_send.c_str()))
      Serial.println("Published");
    else
      Serial.println("Can't Publish");
  }
}

