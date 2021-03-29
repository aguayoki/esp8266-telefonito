#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.
const char* ssid = "=)";
const char* password = "Summ3rtim3";
const char* mqtt_server = "192.168.1.251";  //IP address of your Home assistant server if hosting on HASS or the MQTT server you have
#define mqtt_user "mqtt" //enter your MQTT username
#define mqtt_password "krakozhia" //enter your MQTT password

WiFiClient espClient;
PubSubClient client(espClient);

//Declaire global variables
String strTopic;
String strPayload;
String dial_str;
int needToPrint = 0;
int count;
int rotary = 4;
int hook = 5;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0;
int cleared = 0;
int dial;
int dialHasFinishedRotatingAfterMs = 100;
int debounceDelay = 10;
int dial1State = 0;
int dial2State = 0;
int dial3State = 0;
int dial4State = 0;
int dial5State = 0;
int dial6State = 0;
int dial7State = 0;
int dial8State = 0;
int dial9State = 0;
int dial0State = 0;
int hookState = 0;

const long interval = 10000;        // Interval at which to publish sensor readings

// MQTT topics
const PROGMEM char* MQTT_TEL_DIAL_1 = "loft/telefonito/dial/1";
const PROGMEM char* MQTT_TEL_DIAL_2 = "loft/telefonito/dial/2";
const PROGMEM char* MQTT_TEL_DIAL_3 = "loft/telefonito/dial/3";
const PROGMEM char* MQTT_TEL_DIAL_4 = "loft/telefonito/dial/4";
const PROGMEM char* MQTT_TEL_DIAL_5 = "loft/telefonito/dial/5";
const PROGMEM char* MQTT_TEL_DIAL_6 = "loft/telefonito/dial/6";
const PROGMEM char* MQTT_TEL_DIAL_7 = "loft/telefonito/dial/7";
const PROGMEM char* MQTT_TEL_DIAL_8 = "loft/telefonito/dial/8";
const PROGMEM char* MQTT_TEL_DIAL_9 = "loft/telefonito/dial/9";
const PROGMEM char* MQTT_TEL_DIAL_0 = "loft/telefonito/dial/0";
const PROGMEM char* MQTT_TEL_HOOK = "loft/telefonito/hook";

void setup_wifi() {
  delay(2000);
  // using a mac address list in your router
  Serial.println(WiFi.macAddress());
  // We start by connecting to a WiFi network
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
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  strTopic = String((char*)topic);
  // Potential callback, maybe sound?
}

void reconnect() {
  // Loop until we're reconnected
  Serial.print("Attempting MQTT connection...");
  while (!client.connected()) {
    delay(500);
    Serial.print(".");
    // Attempt to connect
    if (client.connect("esp_telefonito", mqtt_user, mqtt_password)) {
      Serial.println(" connected.");
      // Once connected, publish an announcement...
      client.subscribe("loft/#");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600, SERIAL_8N1, SERIAL_TX_ONLY);
  pinMode(dial, INPUT);
  pinMode(rotary, INPUT);
  pinMode(hook, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.publish(MQTT_TEL_HOOK, "OFF");
  client.publish(MQTT_TEL_DIAL_1, "OFF");
  client.publish(MQTT_TEL_DIAL_2, "OFF");
  client.publish(MQTT_TEL_DIAL_3, "OFF");
  client.publish(MQTT_TEL_DIAL_4, "OFF");
  client.publish(MQTT_TEL_DIAL_5, "OFF");
  client.publish(MQTT_TEL_DIAL_6, "OFF");
  client.publish(MQTT_TEL_DIAL_7, "OFF");
  client.publish(MQTT_TEL_DIAL_8, "OFF");
  client.publish(MQTT_TEL_DIAL_9, "OFF");
  client.publish(MQTT_TEL_DIAL_0, "OFF");
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  int reading = digitalRead(rotary);
  if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
  // the dial isn't being dialed, or has just finished being dialed.
    if (needToPrint) {
      // if it's only just finished being dialed, we need to send the number down the serial
      // line and reset the count. We mod the count by 10 because '0' will send 10 pulses.
      //Serial.print(count % 10, DEC);
      int dial = count % 10;

      
      if(dial == 1) {
        if(dial1State == 0){
          client.publish(MQTT_TEL_DIAL_1, "ON");  
          dial1State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_1, "OFF");  
          dial1State = 0;
        } 
      }
      if(dial == 2) {
        if(dial2State == 0){
          client.publish(MQTT_TEL_DIAL_2, "ON");  
          dial2State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_2, "OFF");  
          dial2State = 0;
        } 
      }
      if(dial == 3) {
        if(dial3State == 0){
          client.publish(MQTT_TEL_DIAL_3, "ON");  
          dial3State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_3, "OFF");  
          dial3State = 0;
        } 
      }
      if(dial == 4) {
        if(dial4State == 0){
          client.publish(MQTT_TEL_DIAL_4, "ON");  
          dial4State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_4, "OFF");  
          dial4State = 0;
        } 
      }
      if(dial == 5) {
        if(dial5State == 0){
          client.publish(MQTT_TEL_DIAL_5, "ON");  
          dial5State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_5, "OFF");  
          dial5State = 0;
        } 
      }
      if(dial == 6) {
        if(dial6State == 0){
          client.publish(MQTT_TEL_DIAL_6, "ON");  
          dial6State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_6, "OFF");  
          dial6State = 0;
        } 
      }
      if(dial == 7) {
        if(dial7State == 0){
          client.publish(MQTT_TEL_DIAL_7, "ON");  
          dial7State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_7, "OFF");  
          dial7State = 0;
        } 
      }
      if(dial == 8) {
        if(dial8State == 0){
          client.publish(MQTT_TEL_DIAL_8, "ON");  
          dial8State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_8, "OFF");  
          dial8State = 0;
        } 
      }
      if(dial == 9) {
        if(dial9State == 0){
          client.publish(MQTT_TEL_DIAL_9, "ON");  
          dial9State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_9, "OFF");  
          dial9State = 0;
        } 
      }
      if(dial == 0) {
        if(dial0State == 0){
          client.publish(MQTT_TEL_DIAL_0, "ON");  
          dial0State = 1;
        } else {
          client.publish(MQTT_TEL_DIAL_0, "OFF");  
          dial0State = 0;
        } 
      }
 
    needToPrint = 0;
    count = -2;
    cleared = 0;
    }
  }

  if (reading != lastState) {
    lastStateChangeTime = millis();
  }
  if ((millis() - lastStateChangeTime) > debounceDelay) {
  // debounce - this happens once it's stablized
    if (reading != trueState) {
    // this means that the switch has either just gone from closed->open or vice versa.
      trueState = reading;
      if (trueState == HIGH) {
      // increment the count of pulses if it's gone high.
        count++;
        needToPrint = 1; // we'll need to print this number (once the dial has finished rotating)
      }
    }
  }
  lastState = reading;

  if(digitalRead(hook) == LOW){
    if(hookState == 0){  
      client.publish(MQTT_TEL_HOOK, "ON");
      hookState = 1;
    } else {
      client.publish(MQTT_TEL_HOOK, "OFF");  
      hookState = 0;
    }
   } 

  client.loop();
}
