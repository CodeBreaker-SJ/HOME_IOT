/*
 * author : seong-ju kim (dev.sj.kim@daum.net)
 * 
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


void callback(char* topic, byte* payload, unsigned int length);
boolean reconnect();

void handleRoot();
void handleReset();
void handleRegister();

ESP8266WebServer server(80);
NetworkConfig netConfig;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastReconnectAttempt = 0;

void setup() 
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  netConfig.StartUp();

  Serial.print("Configuring access point...");
  WiFi.softAP(netConfig.GetNAME(), DEFAULT_PASS);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  
  if (!netConfig.GetSSID().equals(DEFAULT_SSID))
  {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(netConfig.GetSSID());
    
    WiFi.begin(netConfig.GetSSID(), netConfig.GetPASSWORD());
    
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }

    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
    delay(500);
    mqttClient.setServer("192.168.0.100", 1883);
    mqttClient.setCallback(callback);
    lastReconnectAttempt = 0;
  }
 
  server.on("/", handleRoot);
  server.on("/sensor_on", handleReset);
  server.on("/sensor_off", handleReset);
  server.on("/reset", handleReset);
  server.on("/register", handleRegister); 
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();

  if (!netConfig.GetSSID().equals(DEFAULT_SSID))
  {
    if (!mqttClient.connected()) 
    {
      long now = millis();
      if (now - lastReconnectAttempt > 5000) 
      {
        lastReconnectAttempt = now;
        if (reconnect()) 
        {
          Serial.println("reconnect mqtt server...");
          lastReconnectAttempt = 0;
        }
      }
    } 
    else 
    {
      mqttClient.loop();
    }
  }
}

void handleRoot() 
{
  String message = 
"<!DOCTYPE html>"
"<html><body>"
"<form action='/register' method='get'>"
"  SSID:<br>"
"  <input type='text' name='ssid' value='home_iot_device' style='width: 400px; height: 30px;'>"
"  <br>"
"  PASSWORD:<br>"
"  <input type='text' name='password' value='password' style='width: 400px; height: 30px;'>"
"  <br>"
"  NAME:<br>"
"  <input type='text' name='name' value='home_iot_device' style='width: 400px; height: 30px;'>"
"  <p>&nbsp<p>"
"  <input type='submit' value='Register' style='width: 400px; height: 40px;'>"
"</form><p>"
"<form action='/reset' method='get'>"
"  <input type='submit' value='Reset Setting' style='width: 400px; height: 40px;'>"
"</form> "
"</body></html> ";
  
  server.send(200, "text/html", message);
}

void handleReset() 
{
  netConfig.Reset();
  
  server.send(200, "text/html", "<h1>Reset Network Setting!</h1>");
}

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
  
  netConfig.Write(server.arg(0), server.arg(1), server.arg(2));

  String message = "";
  message += "<h1>Successfully Write Network Setting!</h1>";
  message += "<br>";
  message += "ssid: ";
  message += server.arg(0);
  message += "<br>";
  message += "pw: ";
  message += server.arg(1);
  message += "<br>";
  message += "name: ";
  message += server.arg(2);

  server.send(200, "text/html", message);
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

boolean reconnect() 
{
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
    
  if (mqttClient.connect(clientId.c_str())) 
  {
    Serial.println("connected mqtt server...");
    // Once connected, publish an announcement...
    mqttClient.publish("outTopic","hello world");
    // ... and resubscribe
    mqttClient.subscribe("inTopic");
  }
  return mqttClient.connected();
}
/*
  String message = "Number of args received:";
  message += server.args();            //Get number of parameters
  message += "\n";                            //Add a new line
  
  for (int i = 0; i < server.args(); i++) 
  {
    message += "Arg no" + (String)i + " –> ";   //Include the current iteration value
    message += server.argName(i) + ": ";     //Get the name of the parameter
    message += server.arg(i) + "\n";              //Get the value of the parameter
  } 
*/
