

// I am trying to put a webserver on my Wemos D1 mini pro.
//I am using this example for the Arduino code: https://github.com/wemos/Arduino_D1/blob/master/doc/esp8266wifi/server-examples.rst

#include <ESP8266WiFi.h>

const char* ssid = "SmartSwitch";
const char* password = "admin@123";
const int pingPin_1 = D4;
const int TrigPin_1 = D3;

const int pingPin_2 = D2;
const int TrigPin_2 = D1;

long duration, inches, cm;
 

WiFiServer server(80);


void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}


// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage =
 //    String("Refresh: 1\r\n") +
      
      String("HTTP/1.1 200 OK \r") +
//      "Distance:  " + String(Water_Level_1()) +
//            "HTTP/1.1 200 OK\r\n") +
//            "Content-Type: text/html\r\n" +
//            "Connection: close\r\n" +  // the connection will be closed after completion of the response
//"\r\n" +
            "Refresh: 1 \r" +  // refresh the page automatically every 5 sec
//            "\r\n" +
//            "<!DOCTYPE HTML>" +
//            "<html>" +
            "Distance:  " + String(Water_Level_1()) +
//            "</html>" +
            "\r\n";
  return htmlPage;
}


//String Water_Level_1()
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


void loop()
{
  WiFiClient client = server.available();
//  client.println(10);
//  delay(1000);
client.print(Water_Level_1());
client.print("      ");
client.println(Water_Level_2());
client.println(analogRead(A0));

  delay(100);
//  Serial.println(prepareHtmlPage());
//Serial.println(Water_Level_1());
//  // wait for a client (web browser) to connect
//  if (client)
//  {
//    Serial.println("\n[Client connected]");
////    Serial.println(distance);
//    while (client.connected())
//    {
//      // read line by line what the client (web browser) is requesting
//     if (client.available())  
//{  
//        String line = client.readStringUntil('\r');  
//        Serial.print(line);  
//        // wait for end of client's request, that is marked with an empty line  
//        if (line.length() == 1 && line[0] == '\n')  
//        {  
//          client.println(prepareHtmlPage());  
//          break;  
//        }  
//          client.println(prepareHtmlPage()); 
//      } 
//    }
//  }
}
