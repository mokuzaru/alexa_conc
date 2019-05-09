/******************************************
   Script to work with alexa to control 2 relays

   By Mokuzaru
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <Wire.h>

// prototypes
boolean connectWifi();

//on/off callbacks
void lightOneOn();
void lightOneOff();
void lightTwoOn();
void lightTwoOff();

// Change this before you flash
const char* ssid = "Misael";
const char* password = "*******";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;

// Set Relay Pins
int relayOne = 4;
int relayTwo = 5;


void setup()
{

  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    
    //Set relay pins to outputs
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
    delay(2000);

    //Set each relay pin to HIGH this display status messagefor each relay
    digitalWrite(relayOne, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayTwo, HIGH);   // sets relayOne on
    delay(500);
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();
    lightOne->serverLoop();
    lightTwo->serverLoop();
  }
}

void lightOneOn() {
   Serial.print("Switch 1 turn on ...");
  digitalWrite(relayOne, LOW);   // sets relayOne on
}

void lightOneOff() {
   Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne off
}

void lightTwoOn() {
   Serial.print("Switch 2 turn on ...");
  digitalWrite(relayTwo, LOW);   // sets relayTwo on
}

void lightTwoOff() {
   Serial.print("Switch 2 turn off ...");
  digitalWrite(relayTwo, HIGH);   // sets relayTwo Off
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
     Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
  }
  else {
     Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
