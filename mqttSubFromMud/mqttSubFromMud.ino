#include <ESP8266WiFi.h>
#include<PubSubClient.h>
#include <time.h>
#define B_pin D4
// Wifi Connection
const char* ssid = "test";
const char* password = "pcf18885";
const char* topic = "Locker";
char message_buff[100];

// MQTT Server address
const char* mqtt_server = "broker.hivemq.com";
WiFiClient vannilaxx123;
PubSubClient client(vannilaxx123);

void setup() {
  Serial.begin(115200);
  pinMode(B_pin, OUTPUT);
  digitalWrite(B_pin, HIGH);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  //  connection to broker script.
  if (client.connect("arduinoClientSE3")) {
   client.publish(topic,"hello world");
    client.subscribe(topic);   // i should probably just point this to the varialbe above.
  }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Here is what i have been using to handle subscriptions. I took it as a snippet from elsewhere but i cannot credit author as i dont have reference!
    int i = 0;

  Serial.println("Message arrived:  topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));
  
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  
  Serial.println("Payload: " + msgString);
  int state = digitalRead(B_pin);  // get the current state of GPIO1 pin
  if (msgString == "on"){    // if there is a "1" published to any topic (#) on the broker then:
    digitalWrite(B_pin,HIGH);     // set pin to the opposite state 
    Serial.println("Switching LED"); 
  }
  else{
    digitalWrite(B_pin,LOW);     // set pin to the opposite state 
    Serial.println("Switching LED");
  }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClientSE3")) {
    Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic, "hello world");
      // ... and resubscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  }

