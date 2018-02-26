//TODO: passwords to EEPROM, set time zone and year, month, day


#include "libraries/ESP8266HTTPClient.h"
//#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
//String GET = "http://184.106.153.149/update?key=";//TEERFD89BN6SDE19";//8OC2G029OG74M1V9"; //TODO: 8OC2G029OG74M1V9 change to https://thingspeak.com api write key
// https://thingspeak.com/channels/189974
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
// String GET = "GET /update?key=THINGSPEAK_KEY";

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "libraries/WiFiManager.h"         //https://github.com/tzapu/WiFiManager

#include <ESP8266mDNS.h>
//#include <ArduinoOTA.h>
ESP8266WebServer server(80);

char* htmlHeader = "<html><head><meta name=\"viewport\" content=\"width=device-width\"><style type=\"text/css\">button {height:100px;width:100px;font-family:monospace;border-radius:5px;}</style></head><body><h1><a href=/>GROWMAT WEB</a></h1>";
char* htmlFooter = "<hr><a href=/settings>SYSTEM SETTINGS</a></body></html>";
//const char HTTP_STYLE[] PROGMEM  = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";

const char* www_username = "admin";
//const char* www_password = "admin";
char www_password[20];

char serverName[20];
char writeApiKey[20];
unsigned int talkbackID;
char talkbackApiKey[20];



unsigned int errorCounter;

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;


#include <WiFiUdp.h>

//#define LCD
#ifdef LCD
#include <SPI.h>
#include "libraries/Adafruit_GFX.h"
#include "libraries/Adafruit_PCD8544.h"
// Pins
const int8_t RST_PIN = 4; //D2;
const int8_t CE_PIN = 5; //D1;
const int8_t DC_PIN = 12; //D6;
//const int8_t DIN_PIN = D7;  // Uncomment for Software SPI
//const int8_t CLK_PIN = D5;  // Uncomment for Software SPI
const int8_t BL_PIN = 16; //D0;
Adafruit_PCD8544 display = Adafruit_PCD8544(DC_PIN, CE_PIN, RST_PIN);
#endif

#include <EEPROM.h>

/*
unsigned int localPort = 2390;      // local port to listen for UDP packets
//Don't hardwire the IP address or we won't get the benefits of the pool.
//Lookup the IP address for the host name instead
IPAddress timeServerIP(195,113,144,201); // time.nist.gov NTP server
//IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
*/

/*
#include "libraries/RF433.h"
//#define RFRX_PIN A4
#define RFTX_PIN D5 //0 //D3 //14 //D5
// MCE07
byte rf1off[] = {11,30,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,10,4,3,3,11,3,11,3,3,4,3,3,4,3,10,3,11,3,4,3,11,3,10,4,10,3,11,3,11,3,11,3,3,3,11,3,4,3,11,3,10,3,11,3,11,3,3,4,3,3,3,4,188,3,3,3,3,30,29,4,3,3,3,4,10,3,11,3,11,3,3,4,3,3,11,3,3,4,3,3,3,4,3,3,3,4,3,3,11,3,4,3,10,4};
byte rf1on[] = {5,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,4,3,3,4,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,4,3,10,4,10,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte rf2off[] = {16,29,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3,3,3,11,3,11,3,4,2,4,3,3,3,11,3,11,3,4,3,11,3,10,3,11,3,11,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,4,3,3,3,188,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,3,4,3,4,3,10,3,4,3,11,3};
byte rf2on[] = {4,11,3,3,3,11,3,4,3,3,3,11,3,3,3,11,3,11,3,188,3,3,4,2,30,30,3,3,3,4,3,10,3,11,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,3,3,4,3,11,3,10,3,4,3,11,3,11,3,11,3,10,3,11,3,11,3,4,3,11,3,3,3,11,3,3,3,4,3,11,3,3,3,11,3,11,3,90,3,3,3,3,30,30,3,3,3,4,3,10,3,11,3,11,3};
byte rf3off[] = {5,3,3,3,29,30,3,3,4,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,10,3,11,3,11,3,3,4,11,3,3,3,4,3,10,3,11,3,11,3,11,3,3,3,4,3,188,3,3,4,2,30,29,4,3,3,3,4,10,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3};
byte rf3on[] = {5,2,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,3,10,3,11,3,4,3,11,3,11,3,10,3,4,3,3,3,4,3,3,3,11,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte rf4off[] = {4,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,10,3,4,3,11,3,3,3,4,3,11,3,3,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,10,3,11,3,4,3,10,3,4,3,3,3,11,3,4,3};
byte rf4on[] = {4,4,3,10,4,89,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,3,3,4,3,10,3,4,3,11,3,10,3,4,3,11,3,188,3,3,3,3,30,29,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,11,3,3,3,4,3,3,3,4,3};
*/

unsigned int r1Off = 2664494;
unsigned int r1On = 2664495;
unsigned int r2Off = 2664492;
unsigned int r2On = 2664493;
unsigned int r3Off = 2664490;
unsigned int r3On = 2664491;
unsigned int r4Off = 2664486;
unsigned int r4On = 2664487;
unsigned int rAllOff = 2664481;
unsigned int rAllOn = 2664482;



#include "libraries/RCSwitch.h"
RCSwitch rcSwitch = RCSwitch();
//#define RFRX_PIN D3
#define RFTX_PIN D5



#include <WiFiUdp.h>
#include "libraries/NTPClient.h"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#include "libraries/interval.h"
Interval minInterval, secInterval;

unsigned long secCounter = 0;
unsigned long cyclerSecCounter, lightOffSecCounter, lightOnSecCounter = 0;
bool secOverflow;

//#include "libraries/Time.h"
//#include "libraries/DateTime.h"
#include "libraries/Timezone.h"
//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);

#define CONFIG_WIFIAP_PIN D7 //4 //D2
#define CONFIG_WIFI_PIN D6 //5 //D1


//16 //D0
//5 //D1
//4 //D2
//0 //D3
//2 //D4
//12 //D5
//14 //D6
#define ONEWIREBUS_PIN D4 //D8 //13 //D7
#define LED0_PIN D0 //14 //D5
#define LED1_PIN D1 //12 //D6

#include "libraries/OneWire.h"
#include "libraries/DallasTemperature.h"

OneWire oneWire(ONEWIREBUS_PIN);
DallasTemperature oneWireSensors(&oneWire);

float temperature;

#define DHT_PIN D2 //9//6
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#include "libraries/DHT.h"

float analogIn;
float temperatureDHT, humidityDHT;
DHT dht(DHT_PIN, DHTTYPE);



#define DEVICES_NUM 8

#define OUTPUT_BIT 0
#define MANUAL_BIT 1
#define CMD_BIT 2
#define RAIN_BIT 3

#define EEPROM_OFFSET 128 //64 //8

struct Device {
	byte par1;
	byte par2;
	//unsigned int durationSec1;
	byte par3;
	byte par4;
	//unsigned int durationSec2;
	byte flags;
	char name[8];
};

Device devices[DEVICES_NUM];


//0 - light, 1 - fan, 3 - cycler
String getDeviceForm(int i, struct Device devices[]) {
	Device d = devices[i];
	String s = "<form action=/dev><input type=hidden name=id value=";
	s += i;
	s += "><h2>";
	s += i;
	s += ": ";
	s += String(d.name);
	if(bitRead(devices[i].flags, MANUAL_BIT))
		s += " MANUAL";
	else
		s += " AUTO";
	if(bitRead(devices[i].flags, OUTPUT_BIT))
		s += " ON";
	else
		s += " OFF";
	s += "</h2>";

	if(i<3)
		s += "<button type=submit name=cmd value=off>OFF</button>&nbsp;&nbsp;&nbsp;<button type=submit name=cmd value=on>ON</button>&nbsp;&nbsp;&nbsp;<button type=submit name=cmd value=auto>AUTO</button>";
	s += "<hr><h3>Settings:</h3>";
	s += "<hr>NAME<br>";
	//s += "<input name=n value=";
	//s += d.name;
	//s += ">";
	s += d.name;
	if(i==0) {
		s += "<hr>LIGHT ON [HOUR]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>LIGHT ON [MINUTE]<br><input name=par2 value=";
		s += d.par2;
		s += "><hr>LIGHT OFF [HOUR]<br><input name=par3 value=";
		s += d.par3;
		s += "><hr>LIGHT OFF [MINUTE]<br><input name=par4 value=";
		s += d.par4;
		s += ">";
	}
	if(i==1) {
		s += "<hr>FAN ON LIGHT [°C]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>FAN OFF LIGHT [°C]<br><input name=par2 value=";
		s += d.par2;
		s += "><hr>FAN ON DARK [°C]<br><input name=par3 value=";
		s += d.par3;
		s += "><hr>FAN OFF DARK [°C]<br><input name=par4 value=";
		s += d.par4;
		s += ">";
	}
	if(i==2) {
		s += "<hr>CYCLER ON [MINUTES]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>CYCLER ON [SECONDS]<br><input name=par2 value=";
		s += d.par2;
		s += "><hr>CYCLER OFF [MINUTES]<br><input name=par3 value=";
		s += d.par3;
		s += "><hr>CYCLER OFF [SECONDS]<br><input name=par4 value=";
		s += d.par4;
		s += ">";
	}
	if(i==3) {
		s += "<hr>LIGHT MAXIMUM AT DARK [%]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>DELAY [SECOND]<br><input name=par2 value=";
		s += d.par2;
		//s += "><hr>TEMEPRATURE HIGH DARK [°C]<br><input name=par3 value=";
		//s += d.par3;
		//s += "><hr>TEMEPRATURE LOW DARK [°C]<br><input name=par4 value=";
		//s += d.par4;
		s += ">";
	}
	if(i==4) {
		s += "<hr>LIGHT MINIMUM AT LIGHT [%]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>DELAY [SECONDS]<br><input name=par2 value=";
		s += d.par2;
		//s += "><hr>LIGHT COMMAND DEALY [SECONDS]<br><input name=par3 value=";
		//s += d.par3;
		//s += "><hr>TEMEPRATURE LOW DARK [°C]<br><input name=par4 value=";
		//s += d.par4;
		s += ">";
	}
	if(i==5) {
		s += "<hr>TEMERATURE HIGH AT LIGHT [°C]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>TEMEPATURE HIGH AT DARK [°C]<br><input name=par2 value=";
		s += d.par2;
		s += "><hr>DEALY [SECONDS]<br><input name=par3 value=";
		s += d.par3;
		//s += "><hr>DEALY AT DARK [SECONDS]<br><input name=par4 value=";
		//s += d.par4;
		s += ">";
	}
	if(i==6) {
		s += "<hr>TEMEPATURE LOW AT LIGHT [°C]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>TEMEPATURE LOW AT DARK [°C]<br><input name=par2 value=";
		s += d.par2;
		s += "><hr>DEALY [SECONDS]<br><input name=par3 value=";
		s += d.par3;
		//s += "><hr>DEALY AT DARK [SECONDS]<br><input name=par4 value=";
		//s += d.par4;
		s += ">";
	}
	if(i==7) {
		s += "<hr>LIGHT COMMAND DELAY [SECONDS]<br><input name=par1 value=";
		s += d.par1;
		s += ">";
	}
	//s += "<hr>F<br><input name=f value=";
	//s += d.flags;
	//s += ">";
	s += "<hr><button type=submit name=cmd value=set>SET</button>";

	s += "</form>";
	return s;
}

void handleRoot(){
   if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();

  Serial.println("Enter handleRoot");
  String message;
  server.send(200, "text/plain", message);
}

bool ap, led1;

void saveApi() {
	int offset = 8;
	EEPROM.put(offset, serverName);
	offset += sizeof(serverName);
	EEPROM.put(offset, writeApiKey);
	offset += sizeof(writeApiKey);
	EEPROM.put(offset, talkbackApiKey);
	offset += sizeof(talkbackApiKey);
	EEPROM.put(offset, talkbackID);
	offset += sizeof(talkbackID);
	EEPROM.put(offset, www_password);
	EEPROM.put(0, 0);
	EEPROM.commit();
}

void saveInstruments() {
	for(int d=0; d< DEVICES_NUM; d++) {
		EEPROM.put(EEPROM_OFFSET + sizeof(Device) * d, devices[d]);
	}
	EEPROM.put(0, 0);
	EEPROM.commit();
}


void setup() {
  // put your setup code here, to run once:
  //strcpy(writeApiKey, "SGRXDOXDL4F6CIGQ");
  //talkbackID = 18221;
  //strcpy(talkbackApiKey, "ED57BAC7V0GLPWL2");

  Serial.begin(115200);
  Serial.print("\n\n");

#ifdef LCD
  display.begin();
  display.setContrast(60);  // Adjust for your display


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("GROWMAT WEB");
  display.display();
#endif

  oneWireSensors.begin();

  dht.begin();

#ifdef RFRX_PIN
  rcSwitch.enableReceive(RFRX_PIN);
#endif

#ifdef RFTX_PIN
  rcSwitch.enableTransmit(RFTX_PIN);
#endif

  EEPROM.begin(512);
  if(!EEPROM.read(0)) {
  //if(false) {

	  int offset = 8;
	  //for(int i=0; i<20; i++) {
	  //	 EEPROM.get(offset++, writeApiKey[i]);
	  //}
	  //for(int i=0; i<20; i++) {
	  //	 EEPROM.get(offset++, talkbackApiKey[i]);
	  //}
	  EEPROM.get(offset, serverName);
	  offset += sizeof(serverName);
      EEPROM.get(offset, writeApiKey);
      offset += sizeof(writeApiKey);
      EEPROM.get(offset, talkbackApiKey);
      offset += sizeof(talkbackApiKey);
	  EEPROM.get(offset, talkbackID);
	  offset += sizeof(talkbackID);
	  EEPROM.get(offset, www_password);

	  for(int d=0; d< DEVICES_NUM; d++) {
		  EEPROM.get(EEPROM_OFFSET + sizeof(Device) * d, devices[d]);
	  }
  }
  else {
	  strcpy(serverName, "api.thingspeak.com") ;
	  writeApiKey[0] = '/0';
	  talkbackID = 0;
	  talkbackApiKey[0], '/0';
	  strcpy(www_password, "admin") ;
	  saveApi();
	  saveInstruments();
  }

  strcpy(devices[0].name, "LIGHT  ");
  strcpy(devices[1].name, "FAN    ");
  strcpy(devices[2].name, "CYCLER ");
  strcpy(devices[3].name, "ALRM L+");
  strcpy(devices[4].name, "ALRM L-");
  strcpy(devices[5].name, "ALRM T+");
  strcpy(devices[6].name, "ALRM T-");
  strcpy(devices[7].name, "MISC   ");

  pinMode(CONFIG_WIFIAP_PIN, INPUT_PULLUP);
  pinMode(CONFIG_WIFI_PIN, INPUT_PULLUP);
  pinMode(RFTX_PIN, OUTPUT);

#ifdef LED0_PIN
  pinMode(LED0_PIN, OUTPUT);
  digitalWrite(LED0_PIN, LOW);
#endif
#ifdef LED1_PIN
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
#endif
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  //wifiManager.resetSettings();

  //set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(digitalRead(CONFIG_WIFIAP_PIN) == LOW) {
	  ap = true;
	  strcpy(www_password, "admin") ;

#ifdef LED0_PIN
	  digitalWrite(LED0_PIN, HIGH);
	  //digitalWrite(LED1_PIN, HIGH);
#endif

	  WiFi.softAP("GROWMAT-WEB", "GROWMAT-WEB");
	  IPAddress myIP = WiFi.softAPIP();
	  Serial.println("Starting AP ...");
	  Serial.print("AP IP address: ");
	  Serial.println(myIP);

	  Serial.print("Open http://");
	  Serial.print(myIP);
	  Serial.println("/ in your browser to see it working");
	  //AP
  }
  else {

	  if ( digitalRead(CONFIG_WIFI_PIN) == LOW ) {
		  //digitalWrite(LED1_PIN, HIGH);

#ifdef LED0_PIN
		  digitalWrite(LED0_PIN, HIGH);
#endif

		  Serial.println("Starting AP for reconfiguration ...");
      	  wifiManager.startConfigPortal("GROWMAT-WEB");
      	  //AP
  	  }
  	  else {

  		  Serial.println("Starting AP or connecting to Wi-Fi ...");
  		  wifiManager.autoConnect("GROWMAT-WEB");

#ifdef LED1_PIN
  		  digitalWrite(LED1_PIN, HIGH);
#endif

  	  }
  	  //or use this for auto generated name ESP + ChipID
  	  //wifiManager.autoConnect();


  	  //if you get here you have connected to the WiFi
  	  Serial.println("Connected");
  	  Serial.print("Open http://");
  	  Serial.print(WiFi.localIP());
  	  Serial.println("/ in your browser to see it working");

  }
  Serial.println("Ready");

  /*
  //-----------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }*/
  //ArduinoOTA.begin();

  server.on("/", [](){
    Serial.println("/");
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();

    //String message = "<HTML><BODY><H1>esp8266</H1><A HREF=set?state=on>ON</A><BR><A HREF=set?state=off>OFF</A></BODY></HTML>";
    //String message = "<html><head><meta name=\"viewport\" content=\"width=device-width\"></head><body><h1>esp8266</h1>";
    String message = htmlHeader;

    //message += "Time: " + timeClient.getFormattedTime();
    time_t t = CE.toLocal(timeClient.getEpochTime());

    byte h = (t / 3600) % 24;
    byte m = (t / 60) % 60;
    byte s = t % 60;
    message += "<h2>TIME: ";
    if(h<10)
    	message += '0';
    message += h;
    message += ":";
    if(m<10)
        	message += '0';
    message += m;
    message += ":";
    if(s<10)
        	message += '0';
    message += s;
    message += "</h2>";

    message += "<h2>TEMPERATURE DHT: ";
    message += temperatureDHT;
    message += "°C</h2>";

    message += "<h2>HUMIDITY DHT: ";
    message += humidityDHT;
    message += "%</h2>";

    message += "<h2>LIGHT: ";
    message += analogIn;
    message += "%</h2>";

    message += "<h2>TEMPERATURE: ";
    message += temperature;
    message += "°C</h2>";

    for(int i = 0; i < DEVICES_NUM; i++) {
    	message += "<hr><a href=./dev?id=";
    	message += i;
    	message += ">";
    	message += i;
    	message += ": ";
    	message += devices[i].name;
    	//message += "</A> ";
    	if(bitRead(devices[i].flags, MANUAL_BIT))
    		message += " MANUAL";
    	else
    		message += " AUTO";
    	if(bitRead(devices[i].flags, OUTPUT_BIT))
    	    		message += " ON";
    	    	else
    	    		message += " OFF";
    	message += "</a> ";
    }
    message += "</table>";
    //message += "</body></html>";
    message += "<hr><a href=./save>SAVE INSTRUMENTS!</a>";
    message += htmlFooter;
    server.send(200, "text/html", message);
  });

  /*
  server.on("/dev", [](){
	  Serial.println("/dev");
	  byte id=server.arg("id").toInt();
	  String message = "<HTML><BODY><H1>esp8266</H1>";
	  message += getDeviceForm(id, devices);
	  message += "</BODY></HTML>";
	  server.send(200, "text/html", message);
   });
   */

  server.on("/save", [](){
      Serial.println("/save");
      if(!server.authenticate(www_username, www_password))
        return server.requestAuthentication();

      saveInstruments();

      char value;
      for(int i=0; i < 512; i++) {
    	  //EEPROM.write(addr, val);
    	  if(i % 32 == 0)
    		  Serial.println();
    	  value = EEPROM.read(i);
    	  Serial.print(value, HEX);
    	  Serial.print(' ');
      }

      String message = htmlHeader;
      message += "OK";
      message += htmlFooter;
      server.send(200, "text/html", message);

  });

  server.on("/settings", [](){
        Serial.println("/settings");
        if(!server.authenticate(www_username, www_password))
          return server.requestAuthentication();

        time_t t = CE.toLocal(timeClient.getEpochTime());
		byte h = (t / 3600) % 24;
		byte m = (t / 60) % 60;
		byte s = t % 60;

        String message = htmlHeader;
        message += "<hr>";
        message += "<form action=/savesettings>";
        message += "HOURS<br><input name=hours value=";
        //message += timeClient.getHours();
        message += h;
        message += "><br>";
        message += "<br>MINUTES<br><input name=minutes value=";
		//message += timeClient.getMinutes();
        message += m;
		message += "><br>";
        message += "<br>SECONDS<br><input name=seconds value=";
        //message += timeClient.getSeconds();
        message += s;

        message += "><br>";
		message += "<br>YEAR<br><input name=year value=";
		//message += timeClient.getMinutes();
		//message += time_t;
        message += "><br>";
		message += "<br>MONTH<br><input name=month value=";
		//message += timeClient.getMinutes();
		//message += m;
        message += "><br>";
		message += "<br>DAY<br><input name=day value=";
		//message += timeClient.getMinutes();
		//message += m;


        message += "><br><br>";
        message += "<button type=submit name=cmd value=settime>SET TIME</button>";
        message += "</form>";
        message += "<hr>";

        message += "<form action=/savesettings>";
        message += "SERVER NAME<br><input name=servername value=";
		message += serverName;
		message += "><br>";
	    message += "<br>WRITE API KEY<br><input name=writeapikey value=";
	    message += writeApiKey;
	    message += "><br>";
	    message += "<br>TALKBACK ID<br><input name=talkbackid value=";
		message += talkbackID;
		message += "><br>";
	    message += "<br>TALKBACK API KEY<br><input name=talkbackapikey value=";
	    message += talkbackApiKey;
	    message += "><br>";
	    message += "<br>ADMIN PASSWORD<br><input name=www_password value=";
	    message += www_password;
	    message += "><br><br>";
	    message += "<button type=submit name=cmd value=setapi>SET API!</button>";
	    message += "</form>";

        message += htmlFooter;
        server.send(200, "text/html", message);

    });

  server.on("/savesettings", [](){
          Serial.println("/savesettings");

          if(!server.authenticate(www_username, www_password))
            return server.requestAuthentication();

          String message = htmlHeader;
          if(server.arg("cmd").equals("settime")) {
        	  int offset = CE.toUTC(0) - CE.toLocal(0);
        	  //Serial.println(offset);
			  byte h=(unsigned long)server.arg("hours").toInt() + offset / 3600;
			  byte m=(unsigned long)server.arg("minutes").toInt() + offset / 60;
			  byte s=(unsigned long)server.arg("seconds").toInt() + offset;
          	  timeClient.setEpochTime(h * 3600 + m * 60 + s);
          	  message += "TIME SET";
  	  	  }

          if(server.arg("cmd").equals("setapi")) {
        	  strcpy(serverName, server.arg("servername").c_str());
        	  strcpy(writeApiKey, server.arg("writeapikey").c_str());
        	  talkbackID = server.arg("talkbackid").toInt();
        	  strcpy(talkbackApiKey, (char*)server.arg("talkbackapikey").c_str());

        	  //TODO: disable for demo
        	  //strcpy(www_password, server.arg("www_password").c_str());

        	  message += "API SET";

        	  saveApi();
          }

          message += htmlFooter;
          server.send(200, "text/html", message);

      });

  server.on("/dev", [](){
    Serial.println("/dev");
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();

    String cmd=server.arg("cmd");
    Serial.println(cmd);

	byte id=server.arg("id").toInt();
    if(cmd.equals("set")) {
    	byte par1=server.arg("par1").toInt();
    	byte par2=server.arg("par2").toInt();
    	//unsigned int durationSec1=server.arg("d1").toInt();
    	byte par3=server.arg("par3").toInt();
    	byte par4=server.arg("par4").toInt();
    	//unsigned int durationSec2=server.arg("d2").toInt();
    	String name=server.arg("n");

    	if(id >=0 && id < DEVICES_NUM) {
    		devices[id].par1 = par1;
    		devices[id].par2 = par2;
    		//devices[id].durationSec1 = durationSec1;
    		devices[id].par3 = par3;
    		devices[id].par4 = par4;
    		//devices[id].durationSec2 = durationSec2;
    		//name.toCharArray(devices[id].name, 8);
    	}
    }
    if(cmd.equals("auto")) {
    	bitClear(devices[id].flags, MANUAL_BIT);
    }
    if(cmd.equals("off")) {
    	bitSet(devices[id].flags, MANUAL_BIT);
    	bitClear(devices[id].flags, CMD_BIT);
    	bitClear(devices[id].flags, OUTPUT_BIT);
    }
    if(cmd.equals("on")) {
    	bitSet(devices[id].flags, MANUAL_BIT);
    	bitSet(devices[id].flags, CMD_BIT);
    	bitSet(devices[id].flags, OUTPUT_BIT);
    }

	//String message = "<html><head><meta name=\"viewport\" content=\"width=device-width\"><style type=\"text/css\">button {height:100px;width:100px;font-family:monospace;border-radius:5px;}</style></head><body><h1><a href=/>esp8266</a></h1>";
    String message = htmlHeader;
	message += getDeviceForm(id, devices);
	//message += "</body></html>";
	message += htmlFooter;

    server.send(200, "text/html", message);
  });


  server.begin();



  timeClient.update();
}

void loop() {
  // put your main code here, to run repeatedly:

  // ArduinoOTA.handle();
  server.handleClient();

#ifdef LCD
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(WiFi.localIP());
  display.println(timeClient.getFormattedTime());
  display.print(temperature);
  display.print('C');
  display.display();
#endif

  if(minInterval.expired()) {
	  minInterval.set(60000);
	  //minInterval.set(5000);



#ifdef LED1_PIN
	  digitalWrite(LED1_PIN, LOW);
#endif

	  if(!ap)
		  timeClient.update();




	  if(serverName != "" && !ap) {
		  HTTPClient http;  //Declare an object of class HTTPClient

		  int httpCode;
		  do {
			  //http.begin("http://api.thingspeak.com/talkbacks/18221/commands/execute?api_key=ED57BAC7V0GLPWL2");
			  //String getTalkback = "http://api.thingspeak.com/talkbacks/" + String(talkbackID) + "/commands/execute?api_key=" + talkbackApiKey;
			  String getTalkback = "http://" + String(serverName) + "/talkbacks/" + String(talkbackID) + "/commands/execute?api_key=" + talkbackApiKey;
			  Serial.println(getTalkback);
			  http.begin(getTalkback);
			  httpCode = http.GET();                                                                  //Send the request
			  //Serial.println(httpCode);
			  //Serial.println(http.errorToString(httpCode));

			  if (httpCode > 0) { //Check the returning code
				  String payload = http.getString();   //Get the request response payload
				  Serial.println(payload);                     //Pri
				  if(payload == "")
					  break;
				  if(payload == "error_auth_required")
					  break;
				  if(payload.charAt(0)=='L') {
					  if(payload.charAt(1)=='0') {
						  bitClear(devices[0].flags, OUTPUT_BIT);
						  bitSet(devices[0].flags, MANUAL_BIT);
					  }
					  else if(payload.charAt(1)=='1') {
						  bitSet(devices[0].flags, OUTPUT_BIT);
						  bitSet(devices[0].flags, MANUAL_BIT);
					  }
					  else if(payload.charAt(1)=='A') {
						  bitClear(devices[0].flags, MANUAL_BIT);
					  }
				  }
				  if(payload.charAt(0)=='F') {
					  if(payload.charAt(1)=='0') {
						  bitClear(devices[1].flags, OUTPUT_BIT);
						  bitSet(devices[1].flags, MANUAL_BIT);
					  }
					  else if(payload.charAt(1)=='1') {
						  bitSet(devices[1].flags, OUTPUT_BIT);
						  bitSet(devices[1].flags, MANUAL_BIT);
					  }
					  else if(payload.charAt(1)=='A') {
						  bitClear(devices[1].flags, MANUAL_BIT);
					  }
				  }
				  if(payload.charAt(0)=='C') {
					  if(payload.charAt(2)=='0') {
						  bitClear(devices[2].flags, OUTPUT_BIT);
						  bitSet(devices[2].flags, MANUAL_BIT);
					  }
					  else if(payload.charAt(1)=='1') {
						  bitSet(devices[2].flags, OUTPUT_BIT);
						  bitSet(devices[2].flags, MANUAL_BIT);
					  }
					  else if(payload.charAt(2)=='A') {
						  bitClear(devices[2].flags, MANUAL_BIT);
					  }
				  }
			  }
			  else
				  break;
		  } while (true);


		  String alarm = String(bitRead(devices[3].flags, OUTPUT_BIT) * 1 + bitRead(devices[4].flags, OUTPUT_BIT) * 10 + bitRead(devices[5].flags, OUTPUT_BIT) * 100 + bitRead(devices[6].flags, OUTPUT_BIT) * 1000);
		  String l = String(bitRead(devices[0].flags, OUTPUT_BIT) + bitRead(devices[0].flags, MANUAL_BIT) * 10);
		  String f = String(bitRead(devices[1].flags, OUTPUT_BIT) + bitRead(devices[1].flags, MANUAL_BIT) * 10);
		  String c = String(bitRead(devices[2].flags, OUTPUT_BIT) + bitRead(devices[2].flags, MANUAL_BIT) * 10);
		  //String get = GET + "SGRXDOXDL4F6CIGQ" + "&field1=" + alarm +"&field2="+ String(temperature) + "&field3=" + l  + "&field4=" + f;
		  //String get = GET + writeApiKey + "&field1=" + alarm +"&field2="+ String(temperature) + "&field3=" + l  + "&field4=" + f;
		  String get = "http://" + String(serverName) + "/update?key=" + writeApiKey + "&field1=" + alarm + "&field2=" + String(temperatureDHT) +  "&field3="+ String(humidityDHT)  + "&field4="+ String(analogIn) + "&field5="+ String(temperature) + "&field6=" + l + "&field7=" + f + "&field8="  + c + "&field9=" + millis() +  "&field10=" + errorCounter +  "&field11=" + httpCode;
		  Serial.println(get);
		  http.begin(get);  //Specify request destination
		  httpCode = http.GET();                                                                  //Send the request
		  //Serial.println(httpCode);
		  //Serial.println(http.errorToString(httpCode));
		  if(httpCode != 200)
			  errorCounter++;

		  if (httpCode > 0) { //Check the returning code
			String payload = http.getString();   //Get the request response payload
			Serial.println(payload);                     //Print the response payload
		  }

		  http.end();   //Close connection
  	  }

#ifdef LED1_PIN
	  digitalWrite(LED1_PIN, HIGH);
#endif

  }



  if (secInterval.expired()) {
  		secInterval.set(1000);
  		if(ap) {
#ifdef LED1_PIN
  			digitalWrite(LED1_PIN, led1);
#endif
  			led1 = !led1;
  		}

  		if(secCounter == 0)
			secOverflow = true;

  		if((secCounter & 0xF) == 0xF) {
			oneWireSensors.requestTemperatures(); // Send the command to get temperatures
			temperature = oneWireSensors.getTempCByIndex(0);
			//Serial.println(temperature);

			float t0, h0;
			t0 = dht.readTemperature();// + DHT_TEMP_CORRECTION;
			h0 = dht.readHumidity();
			if(!isnan(t0)) temperatureDHT = t0;
			if(!isnan(h0)) humidityDHT = h0;

			analogIn = 4095 / analogRead(A0) ;
			/*
			Serial.print(t0);
			Serial.print('\t');
			Serial.print(h0);
			Serial.print('\t');
			Serial.println(analogIn);
			 */
  		}

#ifdef RFRX_PIN
		if (rcSwitch.available()) {
			Serial.print(rcSwitch.getReceivedValue());
			Serial.print('\t');
			Serial.print(rcSwitch.getReceivedBitlength());
			Serial.print('\t');
			Serial.print(rcSwitch.getReceivedDelay());
			Serial.print('\t');
			unsigned int* p = rcSwitch.getReceivedRawdata();
			for(int i = 0; i < RCSWITCH_MAX_CHANGES; i++)
				Serial.print(*(p + i));
			Serial.print('\t');
			Serial.print(rcSwitch.getReceivedProtocol());
		    rcSwitch.resetAvailable();
		    Serial.println();
		 }
#endif


		if(bitRead(devices[0].flags, OUTPUT_BIT)) {
			if(lightOnSecCounter < -1)
					lightOnSecCounter++;
			lightOffSecCounter = 0;
		}
		else {
			if(lightOffSecCounter < -1)
				lightOffSecCounter++;
			lightOnSecCounter = 0;
		}
  		//Serial.print('\n');
  		for(int i = 0; i < DEVICES_NUM; i++) {
			if(i == 0) {
				unsigned int onSec1 = devices[i].par1 * 3600 + devices[i].par2 * 60;
				unsigned int offSec1 = devices[i].par3 * 3600 + devices[i].par4 * 60;
				//unsigned int offSec1 = onSec1 + devices[i].durationSec1;
				//unsigned int onSec2 = devices[i].par3 * 3600 + devices[i].par4 * 60;
				//unsigned int offSec2 = onSec2 + devices[i].durationSec2;

				//unsigned int onSec = timeClient.getHours() * 3600 + timeClient.getMinutes() * 60;
				time_t t = CE.toLocal(timeClient.getEpochTime());
				byte h = (t / 3600) % 24;
				byte m = (t / 60) % 60;
				byte s = t % 60;
				unsigned int onSec = h * 3600 + m * 60;

				unsigned int offSec = onSec;
				/*
				Serial.print(onSec);
				Serial.print('\t');
				Serial.print(offSec);
				Serial.print('\t');

				Serial.print(onSec1);
				Serial.print('\t');
				Serial.print(offSec1);
				Serial.print('\n');
				 */
				bool out1 = false;
				bool out2 = false;

				if(offSec1 >= 24 * 60 * 60)
					offSec = onSec + 24 * 60 * 60;
				if(onSec >= onSec1 && offSec < offSec1) {
					out1 = true;
				}
				//if(offSec2 >= 24 * 60 * 60)
				//	offSec = onSec + 24 * 60 * 60;
				//if(onSec >= onSec2 && offSec < offSec2) {
				//	out2 = true;
				//}

				if(!bitRead(devices[i].flags, MANUAL_BIT)) {
					if(out1 || out2)
						bitSet(devices[i].flags, OUTPUT_BIT);
					else
						bitClear(devices[i].flags, OUTPUT_BIT);
				}
  			}

			if(i==1) {
				if(!bitRead(devices[1].flags, MANUAL_BIT)) {
					if(bitRead(devices[0].flags, OUTPUT_BIT)) {
						//light
						if(temperature >= devices[1].par1)
							bitSet(devices[1].flags, OUTPUT_BIT);
						if(temperature <= devices[1].par2)
							bitClear(devices[1].flags, OUTPUT_BIT);
					}
					else {
						//dark
						if(temperature >= devices[1].par3)
							bitSet(devices[1].flags, OUTPUT_BIT);
						if(temperature <= devices[1].par4)
							bitClear(devices[1].flags, OUTPUT_BIT);
					}
				}

			}

			if(i==2) {
				//TODO: cycler
				if(!bitRead(devices[2].flags, OUTPUT_BIT)) {
					if(cyclerSecCounter > devices[2].par1 * 60 + devices[2].par2) {
						bitSet(devices[2].flags, OUTPUT_BIT);
						cyclerSecCounter = 0;
					}
				}
				else {
					if(cyclerSecCounter > devices[2].par3 * 60 + devices[2].par4) {
						bitClear(devices[2].flags, OUTPUT_BIT);
						cyclerSecCounter = 0;
					}
				}
				cyclerSecCounter++;
			}
			if(i==3) {
				if(!bitRead(devices[0].flags, OUTPUT_BIT) && lightOffSecCounter > devices[3].par2) {
					//dark
					if(analogIn >= devices[3].par1)
						bitSet(devices[3].flags, OUTPUT_BIT);
					if(analogIn <= devices[3].par1 - 5.0)
						bitClear(devices[3].flags, OUTPUT_BIT);
				}
				else {
					//light
					bitClear(devices[3].flags, OUTPUT_BIT);
				}
			}
			if(i==4) {
				if(bitRead(devices[0].flags, OUTPUT_BIT && lightOnSecCounter > devices[4].par2)) {
					//light
					if(analogIn <= devices[4].par1)
						bitSet(devices[4].flags, OUTPUT_BIT);
					if(analogIn >= devices[4].par1 + 5.0)
						bitClear(devices[4].flags, OUTPUT_BIT);
				}
				else {
					//dark
					bitClear(devices[4].flags, OUTPUT_BIT);
				}
			}

			if(i==5) {
				if(bitRead(devices[0].flags, OUTPUT_BIT) || lightOnSecCounter > devices[5].par3) {
					//light
					if(temperature >= devices[5].par1)
						bitSet(devices[5].flags, OUTPUT_BIT);
					if(temperature <= devices[5].par1 - 0.5)
						bitClear(devices[5].flags, OUTPUT_BIT);
				}
				else {
					//dark
					if(temperature >= devices[5].par2)
						bitSet(devices[5].flags, OUTPUT_BIT);
					if(temperature <= devices[5].par2 - 0.5)
						bitClear(devices[5].flags, OUTPUT_BIT);
				}
			}
			if(i==6) {
				if(bitRead(devices[0].flags, OUTPUT_BIT) || lightOffSecCounter < devices[5].par3) {
					//light
					if(temperature <= devices[6].par1)
						bitSet(devices[6].flags, OUTPUT_BIT);
					if(temperature >= devices[6].par1 + 0.5)
						bitClear(devices[6].flags, OUTPUT_BIT);
				}
				else {
					//dark
					if(temperature <= devices[6].par2)
						bitSet(devices[6].flags, OUTPUT_BIT);
					if(temperature >= devices[6].par2 + 0.5)
						bitClear(devices[6].flags, OUTPUT_BIT);
				}

			}

  			//Serial.print(devices[i].flags);
  			//Serial.print('\t');
  			//Serial.print(devices[i].name);
  			//Serial.print('\n');


  		}

  		if((secCounter & 0xF) == 0xF) {
#ifdef RFTX_PIN
  			//light protection
  			if(secCounter > devices[7].par1 || secOverflow || bitRead(devices[0].flags, MANUAL_BIT)) {
				if(bitRead(devices[0].flags, OUTPUT_BIT))
					//sendSignal(RFTX_PIN, rf1on);
					rcSwitch.send(r1On, 24);
				else
					//sendSignal(RFTX_PIN, rf1off);
					rcSwitch.send(r1Off, 24);
			}
			if(bitRead(devices[1].flags, OUTPUT_BIT))
				rcSwitch.send(r2On, 24);
			else
				rcSwitch.send(r2Off, 24);
			if(bitRead(devices[2].flags, OUTPUT_BIT))
				rcSwitch.send(r3On, 24);
			else
				rcSwitch.send(r3Off, 24);
#endif
			//secCounter = 0;
  		}
  		secCounter++;


#ifdef LED0_PIN
		digitalWrite(LED0_PIN, bitRead(devices[3].flags, OUTPUT_BIT) || bitRead(devices[4].flags, OUTPUT_BIT) || bitRead(devices[5].flags, OUTPUT_BIT) || bitRead(devices[6].flags, OUTPUT_BIT));
#endif
  }


  //get a random server from the pool
  //WiFi.hostByName(ntpServerName, timeServerIP);

  //sendNTPpacket(timeServerIP); // send an NTP packet to a time server
   // wait to see if a reply is available
   //delay(1000);
/*
   int cb = udp.parsePacket();
   if (!cb) {
     Serial.println("no packet yet");
   }
   else {
     Serial.print("packet received, length=");
     Serial.println(cb);
     // We've received a packet, read the data from it
     udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

     //the timestamp starts at byte 40 of the received packet and is four bytes,
     // or two words, long. First, esxtract the two words:

     unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
     unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
     // combine the four bytes (two words) into a long integer
     // this is NTP time (seconds since Jan 1 1900):
     unsigned long secsSince1900 = highWord << 16 | lowWord;
     Serial.print("Seconds since Jan 1 1900 = " );
     Serial.println(secsSince1900);

     // now convert NTP time into everyday time:
     Serial.print("Unix time = ");
     // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
     const unsigned long seventyYears = 2208988800UL;
     // subtract seventy years:
     unsigned long epoch = secsSince1900 - seventyYears;
     // print Unix time:
     Serial.println(epoch);


     // print the par1, par2 and second:
     Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
     Serial.print((epoch  % 86400L) / 3600); // print the par1 (86400 equals secs per day)
     Serial.print(':');
     if ( ((epoch % 3600) / 60) < 10 ) {
       // In the first 10 par2s of each par1, we'll want a leading '0'
       Serial.print('0');
     }
     Serial.print((epoch  % 3600) / 60); // print the par2 (3600 equals secs per par2)
     Serial.print(':');
     if ( (epoch % 60) < 10 ) {
       // In the first 10 seconds of each par2, we'll want a leading '0'
       Serial.print('0');
     }
     Serial.println(epoch % 60); // print the second
   }
   // wait ten seconds before asking for the time again
   delay(10000);*/
}
/*
// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("Sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
*/
