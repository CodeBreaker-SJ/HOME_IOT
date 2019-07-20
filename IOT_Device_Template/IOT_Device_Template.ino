/*
 * author : seong-ju kim (dev.sj.kim@daum.net)
 * 
MIT License

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include "NetworkConfig.h"

void updateMQTT();
void handleMQTT(char* topic, byte* payload, unsigned int length);
boolean reconnectMQTT();

void updateWeb();
void handleRoot();
void handleReset();
void handleRegister();


ESP8266WebServer server(80);
NetworkConfig netConfig;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastReconnectAttempt = 0;
long lastSend = 0;

void setup() 
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //설정 읽어오기
  netConfig.StartUp();

  //AP 모드 시작
  Serial.print("Configuring access point...");
  WiFi.softAP(netConfig.GetNAME(), DEFAULT_PASS);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);


  //웹서버 시작
  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.on("/register", handleRegister); 
  server.begin();
  
  Serial.println("HTTP server started");

  Serial.println(netConfig.Get_MQTT_IP().c_str());
  IPAddress addr;
  if (addr.fromString(netConfig.Get_MQTT_IP())) 
  {
    //mqttClient.setServer(netConfig.Get_MQTT_IP().c_str(), 1883);
    mqttClient.setServer(addr, 1883);
    mqttClient.setCallback(handleMQTT);
    lastReconnectAttempt = 0;
    lastSend = 0;
  }
  
  //Wifi 시작
  if (!netConfig.GetSSID().equals(DEFAULT_SSID))
  {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(netConfig.GetSSID());
    
    WiFi.begin(netConfig.GetSSID(), netConfig.GetPASSWORD());

    int wifiConnectAttempt = 0;
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(50);
      Serial.print(".");
      wifiConnectAttempt++;
      if (wifiConnectAttempt > 200)
        break;
    }

    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
    delay(500);
     
  }
}

void loop() 
{
  updateWeb();
  updateMQTT();
}

//웹서버 업데이트
void updateWeb()
{
  //업데이트 웹서버
  server.handleClient();
}

//웹서버 (192.168.0.x/ 요청)
void handleRoot() 
{
  String message = 
"<!DOCTYPE html>"
"<html><head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"</head><body>"
"<form action='/register' method='get'>"
"  SSID:<br>"
"  <input type='text' name='ssid' value='iot_ssid' style='width: 400px; height: 30px;'>"
"  <br>"
"  PASSWORD:<br>"
"  <input type='text' name='password' value='password' style='width: 400px; height: 30px;'>"
"  <br>"
"  NAME:<br>"
"  <input type='text' name='name' value='iot_device' style='width: 400px; height: 30px;'>"
"  <br>"
"  MQTT_IP:<br>"
"  <input type='text' name='mqtt_ip' value='192.168.0.100' style='width: 400px; height: 30px;'>"
"  <p>&nbsp<p>"
"  <input type='submit' value='Register' style='width: 400px; height: 40px;'>"
"</form><p>"
"<form action='/reset' method='get'>"
"  <input type='submit' value='Reset Setting' style='width: 400px; height: 40px;'>"
"</form> "
"</body></html> ";
  
  server.send(200, "text/html", message);
}

//웹서버 (192.168.0.x/Reset/ 요청)
void handleReset() 
{
  netConfig.Reset();
  
  server.send(200, "text/html", "<h1>Reset Network Setting!</h1>");
}

//웹서버 (192.168.0.x/Register/ 요청)
void handleRegister() 
{
  if (3 > server.args())
    return;
  
  if (!server.argName(0).equals("ssid"))
    return;

  if (!server.argName(1).equals("password"))
    return;

  if (!server.argName(2).equals("name"))
    return;

  if (!server.argName(3).equals("mqtt_ip"))
    return;
  
  netConfig.Write(server.arg(0), server.arg(1), server.arg(2), server.arg(3));

  int wifiConnectAttempt = 0;
  WiFi.begin(netConfig.GetSSID(), netConfig.GetPASSWORD());
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(50);
    Serial.print(".");
    wifiConnectAttempt++;
    if (wifiConnectAttempt > 200)
        break;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
  }
  
  String message = "";
  message += "<html><head>";
  message += "<meta charset='UTF-8'><meta http-equiv='refresh' content='10;url=http://192.168.4.1/'>";
  message += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  message += "</head><body>";
  message += "<h1>네트워크 설정 성공!</h1>";
  message += "<br>";
  message += "ssid: ";
  message += server.arg(0);
  message += "<br>";
  message += "pw: ";
  message += server.arg(1);
  message += "<br>";
  message += "name: ";
  message += server.arg(2);
  message += "<br>";
  message += "mqtt_ip: ";
  message += server.arg(3);
  message += "<br>";
  message += "my_ip: ";
  message += WiFi.localIP().toString();
  message += "</body></html>";

  server.send(200, "text/html", message);
}

//업데이트 MQTT
void updateMQTT()
{
  
  //업데이트 MQTT
  if (netConfig.GetSSID().equals(DEFAULT_SSID))
    return;
    
  
    //재접속 시도
    if (!mqttClient.connected()) 
    {
      //Serial.println("reconnect mqtt server22...");
      long now = millis();
      if ((now - lastReconnectAttempt) > 5000) 
      {
        lastReconnectAttempt = now;
        if (reconnectMQTT()) 
        {
          Serial.println("reconnect mqtt server...");
          lastReconnectAttempt = 0;
        }
      }
    } 
    else //5초 단위 보고
    {
      //
      long now = millis();
      if ((now - lastSend) > 5000) 
      {
        lastSend = now;
        mqttClient.publish("DeviceToServer", "hello world");
        Serial.println("update mqtt...");
      }
      
      mqttClient.loop();
    }
}

//MQTT 요청
void handleMQTT(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//MQTT 재접속
boolean reconnectMQTT() 
{
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  //String clientId = netConfig.GetNAME();   
  if (mqttClient.connect(clientId.c_str())) 
  {
    Serial.println("connected mqtt server...");
    mqttClient.subscribe("ServerToDevice");
  }
  
  return mqttClient.connected();
}
