#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include "FS.h"
#include <SPIFFSIniFile.h>

#define FILE_READ       "r"
#define FILE_WRITE      "w"
#define FILE_APPEND     "a"

#define DEFAULT_SSID     "home_iot_device"
#define DEFAULT_PASS     "password"

class NetworkConfig
{
protected:
  String _name;
  String _ssid;
  String _pass;

public:
  NetworkConfig();
  void StartUp();
  void Shutdown();
  void Reset();
  void Read();
  void Write(String ssid, String pass, String name);
  String GetNAME();
  String GetSSID();
  String GetPASSWORD();
};

#endif
