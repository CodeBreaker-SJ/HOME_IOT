#include "NetworkConfig.h"

NetworkConfig::NetworkConfig() : _name(DEFAULT_SSID), _ssid(DEFAULT_SSID), _pass(DEFAULT_PASS)
{
  
}

void NetworkConfig::StartUp()
{
  if (!SPIFFS.begin()) 
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if (!SPIFFS.exists("/ssid.txt"))
  {
    this->Reset();
  }

  this->Read();
}

void NetworkConfig::Shutdown()
{
  SPIFFS.end();
}

void NetworkConfig::Reset()
{
  this->Write(DEFAULT_SSID, DEFAULT_PASS, DEFAULT_SSID);
}

void NetworkConfig::Read()
{
  File file = SPIFFS.open("/ssid.txt", FILE_READ);
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }
  
  _ssid = "";
  while(file.available()) 
    _ssid += (char)file.read();
  
  file.close();

  Serial.println("File Content:");
  Serial.println(_ssid);

  //////////////////////////////////////////////////////////////////////

  file = SPIFFS.open("/pw.txt", FILE_READ);
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }

  _pass = "";
  while(file.available())
    _pass += (char)file.read();

  file.close();
  
  Serial.println("File Content:");
  Serial.println(_pass);

  //////////////////////////////////////////////////////////////////////

  file = SPIFFS.open("/name.txt", FILE_READ);
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }

  _name = "";
  while(file.available())
    _name += (char)file.read();

  file.close();
  
  Serial.println("File Content:");
  Serial.println(_name);

  //////////////////////////////////////////////////////////////////////

  Serial.println("NetworkConfig ReadData Success..");
}

void NetworkConfig::Write(String ssid, String pass, String name)
{
  _ssid = ssid;
  _pass = pass;
  _name = name;

  File file = SPIFFS.open("/ssid.txt", FILE_WRITE);
 
  if (!file) 
  {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  
  if (file.print(ssid)) 
    Serial.println("File was written");
  else 
    Serial.println("File write failed");

  file.close();

  Serial.println(_ssid);

  //////////////////////////////////////////////////////////////////////

  file = SPIFFS.open("/pw.txt", FILE_WRITE);
 
  if (!file) 
  {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  
  if (file.print(pass)) 
    Serial.println("File was written");
  else 
    Serial.println("File write failed");

  file.close();

  Serial.println(_pass);


  //////////////////////////////////////////////////////////////////////

  file = SPIFFS.open("/name.txt", FILE_WRITE);
 
  if (!file) 
  {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  
  if (file.print(name)) 
    Serial.println("File was written");
  else 
    Serial.println("File write failed");

  file.close();

  Serial.println(_name);
  
  Serial.println("NetworkConfig WriteData Success..");
}

String NetworkConfig::GetNAME()
{
  return String(_name);
}

String NetworkConfig::GetSSID()
{
  return String(_ssid);
}

String NetworkConfig::GetPASSWORD()
{
  return String(_pass);
}
