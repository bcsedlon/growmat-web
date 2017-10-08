#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-10-08 18:20:36

#include "Arduino.h"
#include "libraries/ESP8266HTTPClient.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "libraries/WiFiManager.h"
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "libraries/RF433.h"
#include <WiFiUdp.h>
#include "libraries/NTPClient.h"
#include "libraries/interval.h"
#include "libraries/Timezone.h"
#include "libraries/OneWire.h"
#include "libraries/DallasTemperature.h"
String getDeviceForm(int i, struct Device devices[]) ;
void handleRoot();
void saveApi() ;
void saveInstruments() ;
void setup() ;
void loop() ;

#include "growmatwebESP.ino"


#endif
