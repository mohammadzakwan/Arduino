#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;


#define USE_SERIAL Serial1

#define MESSAGE_INTERVAL 30000
#define HEARTBEAT_INTERVAL 25000

uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;
const char* SSID = "SmartSwitch";
const char* SSID_PWD = "admin@123";
const char* WEBSCOKET_HOST = "192.168.137.1";
const int WEBSOCKET_PORT = 8080;

const int pingPin_1 = D4;
const int TrigPin_1 = D3;

const int pingPin_2 = D2;
const int TrigPin_2 = D1;

long duration, inches, cm;
 
/**
 * Websocket Incomming events handler. Anything that this arduino 
 * receveis is going through this function.
 * 
 */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            isConnected = false;
            break;
        case WStype_CONNECTED:
            {
                USE_SERIAL.printf("[WSc] Connected to url: %s\n",  payload);
                isConnected = true;
                // send message to server when Connected
                // socket.io upgrade confirmation message (required)
                webSocket.sendTXT("5");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[WSc] get text: %s\n", payload);

            // send message Back to server
            webSocket.sendTXT("Next line is echo what Arduino Received Message:");
            webSocket.sendTXT(payload);
            if (strncmp((char *)payload, "GET_WATER_LEVEL", length)) {
              webSocket.sendTXT(getJsonWaterLevel().c_str());
            } else {
              webSocket.sendTXT("Nothing requested from server");
            }
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);

            // send data to server
            webSocket.sendBIN(payload, length);
            break;
    }

}

// TODO: Not sure if this is required in your case.
void boot_wait() {
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
}

void connect_to_wifi() {
    WiFiMulti.addAP(SSID, SSID_PWD);

    // TODO: Not sure if disconnect also required.
    WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }
}
void setup() {
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);
    boot_wait();
    connect_to_wifi();
    webSocket.begin(WEBSCOKET_HOST, WEBSOCKET_PORT);
    //webSocket.setAuthorization("user", "Password");
     // HTTP Basic Authorization
    webSocket.onEvent(webSocketEvent);

}

long Water_Level_1()
{

String Level= String(inches);
  pinMode(pingPin_1, OUTPUT);
  digitalWrite(pingPin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin_1, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin_1, LOW);

  
  pinMode(TrigPin_1, INPUT);
  duration = pulseIn(TrigPin_1, HIGH);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
//return Level;
return inches;
}

long Water_Level_2()
{

String Level= String(inches);
  pinMode(pingPin_2, OUTPUT);
  digitalWrite(pingPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin_2, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin_2, LOW);

  pinMode(TrigPin_2, INPUT);
  duration = pulseIn(TrigPin_2, HIGH);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
//return Level;
return inches;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

String getJsonWaterLevel() {
 return "[\"water_levels\",{\"level_1\":"+String(Water_Level_1())+", \"level_2\":"+String(Water_Level_2())+"}]";
}
void loop() {
    webSocket.loop();

    if(isConnected) {

        uint64_t now = millis();

        if(now - messageTimestamp > MESSAGE_INTERVAL) {
            messageTimestamp = now;
            // example socket.io message with type "messageType" 
            // and JSON payload
            webSocket.sendTXT(getJsonWaterLevel().c_str());
        }
        if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
            heartbeatTimestamp = now;
            // socket.io heartbeat message
            // This is a way to tell server that i am connected.
            // Usually server kick off if someone not send anything.
            // But i have nothing define in server, so not used.
            webSocket.sendTXT("2");
        }
    }
}
