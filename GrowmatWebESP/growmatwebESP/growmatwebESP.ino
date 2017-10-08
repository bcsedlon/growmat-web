

#include "libraries/ESP8266HTTPClient.h"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
String GET = "http://184.106.153.149/update?key=";//TEERFD89BN6SDE19";//8OC2G029OG74M1V9"; //TODO: 8OC2G029OG74M1V9 change to https://thingspeak.com api write key
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
char* htmlFooter = "</body></html>";
//const char HTTP_STYLE[] PROGMEM  = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";

const char* www_username = "admin";
const char* www_password = "admin";

#include <WiFiUdp.h>

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
#include "libraries/RF433.h"
//#define RFRX_PIN A4
#define RFTX_PIN 0 //D3 //14 //D5
// MCE07
byte rf1off[] = {11,30,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,10,4,3,3,11,3,11,3,3,4,3,3,4,3,10,3,11,3,4,3,11,3,10,4,10,3,11,3,11,3,11,3,3,3,11,3,4,3,11,3,10,3,11,3,11,3,3,4,3,3,3,4,188,3,3,3,3,30,29,4,3,3,3,4,10,3,11,3,11,3,3,4,3,3,11,3,3,4,3,3,3,4,3,3,3,4,3,3,11,3,4,3,10,4};
byte rf1on[] = {5,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,4,3,3,4,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,4,3,10,4,10,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte rf2off[] = {16,29,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3,3,3,11,3,11,3,4,2,4,3,3,3,11,3,11,3,4,3,11,3,10,3,11,3,11,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,4,3,3,3,188,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,3,4,3,4,3,10,3,4,3,11,3};
byte rf2on[] = {4,11,3,3,3,11,3,4,3,3,3,11,3,3,3,11,3,11,3,188,3,3,4,2,30,30,3,3,3,4,3,10,3,11,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,3,3,4,3,11,3,10,3,4,3,11,3,11,3,11,3,10,3,11,3,11,3,4,3,11,3,3,3,11,3,3,3,4,3,11,3,3,3,11,3,11,3,90,3,3,3,3,30,30,3,3,3,4,3,10,3,11,3,11,3};
byte rf3off[] = {5,3,3,3,29,30,3,3,4,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,10,3,11,3,11,3,3,4,11,3,3,3,4,3,10,3,11,3,11,3,11,3,3,3,4,3,188,3,3,4,2,30,29,4,3,3,3,4,10,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3};
byte rf3on[] = {5,2,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,3,10,3,11,3,4,3,11,3,11,3,10,3,4,3,3,3,4,3,3,3,11,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte rf4off[] = {4,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,10,3,4,3,11,3,3,3,4,3,11,3,3,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,10,3,11,3,4,3,10,3,4,3,3,3,11,3,4,3};
byte rf4on[] = {4,4,3,10,4,89,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,3,3,4,3,10,3,4,3,11,3,10,3,4,3,11,3,188,3,3,3,3,30,29,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,11,3,3,3,4,3,3,3,4,3};

#include <WiFiUdp.h>
#include "libraries/NTPClient.h"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#include "libraries/interval.h"
Interval minInterval, secInterval;

//#include "libraries/Time.h"
//#include "libraries/DateTime.h"
#include "libraries/Timezone.h"
//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);

#define CONFIG_WIFIAP_PIN 4 //D2
#define CONFIG_WIFI_PIN 5	//D1


//16 //D0
//5 //D1
//4 //D2
//0 //D3
//2 //D4
//12 //D5
//14 //D6
#define ONEWIREBUS_PIN 13 //D7
#define LED0_PIN 14 //5
#define LED1_PIN 12 //6

#include "libraries/OneWire.h"
#include "libraries/DallasTemperature.h"

OneWire oneWire(ONEWIREBUS_PIN);
DallasTemperature oneWireSensors(&oneWire);

float temperature;

#define DEVICES_NUM 4

#define OUTPUT_BIT 0
#define MANUAL_BIT 1
#define CMD_BIT 2
#define RAIN_BIT 3

#define EEPROM_OFFSET 8

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


//0 - light, 1 - fan, 3 - alarm
String getDeviceForm(int i, struct Device devices[]) {
	Device d = devices[i];
	String s = "<form action=/dev><input type=hidden name=id value=";
	s += i;
	s += "><h2>";
	s += i;
	s += ": ";
	s += String(d.name);
	s += "</h2>";
	if(i<2)
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
		s += "<hr>TEMEPRATURE HIGH LIGHT [°C]<br><input name=par1 value=";
		s += d.par1;
		s += "><hr>TEMEPRATURE HIGH DARK [°C]<br><input name=par2 value=";
		s += d.par2;
		//s += "><hr>TEMEPRATURE HIGH DARK [°C]<br><input name=par3 value=";
		//s += d.par3;
		//s += "><hr>TEMEPRATURE LOW DARK [°C]<br><input name=par4 value=";
		//s += d.par4;
		s += ">";
	}
	if(i==3) {
			s += "<hr>TEMEPRATURE LOW LIGHT [°C]<br><input name=par1 value=";
			s += d.par1;
			s += "><hr>TEMEPRATURE LOW DARK [°C]<br><input name=par2 value=";
			s += d.par2;
			//s += "><hr>TEMEPRATURE HIGH DARK [°C]<br><input name=par3 value=";
			//s += d.par3;
			//s += "><hr>TEMEPRATURE LOW DARK [°C]<br><input name=par4 value=";
			//s += d.par4;
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

void setup() {
  // put your setup code here, to run once:


  Serial.begin(115200);
  Serial.print("\n\n");

  oneWireSensors.begin();

  EEPROM.begin(512);
  if(!EEPROM.read(0)) {
	  for(int d=0; d< DEVICES_NUM; d++) {
		  EEPROM.get(EEPROM_OFFSET + sizeof(Device) * d, devices[d]);
	  }
  }

  strcpy(devices[0].name, "LIGHT");
  strcpy(devices[1].name, "FAN");
  strcpy(devices[2].name, "ALRM T+");
  strcpy(devices[3].name, "ALRM T-");

  pinMode(CONFIG_WIFIAP_PIN, INPUT_PULLUP);
  pinMode(CONFIG_WIFI_PIN, INPUT_PULLUP);
  pinMode(RFTX_PIN, OUTPUT);

  pinMode(LED0_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED0_PIN, LOW);
  digitalWrite(LED1_PIN, LOW);

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
	  digitalWrite(LED0_PIN, HIGH);
	  //digitalWrite(LED1_PIN, HIGH);

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
		  digitalWrite(LED0_PIN, HIGH);
		  Serial.println("Starting AP for reconfiguration ...");
      	  wifiManager.startConfigPortal("GROWMAT-WEB");
      	  //AP
  	  }
  	  else {

  		  Serial.println("Starting AP or connecting to Wi-Fi ...");
  		  wifiManager.autoConnect("GROWMAT-WEB");
  		  digitalWrite(LED1_PIN, HIGH);
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
    message += "<hr><a href=./save>SAVE SETTINGS!</a>";
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

      for(int d=0; d< DEVICES_NUM; d++) {
    	  //for(int j=0; j < sizeof(struct Device); j++) {
    	//	  EEPROM.write((i * sizeof(struct Device)) + j, (unsigned char)(**devices + i * sizeof(struct Device) + j));
    	//  }
    	  EEPROM.put(EEPROM_OFFSET + sizeof(Device) * d, devices[d]);
    	  //for(unsigned int i = 0; i<sizeof(Device); i++){
    		  // +i has to be outside of the parentheses in order to increment the address
    		  // by the size of a char. Otherwise you would increment by the size of
    		  // struct_data. You also have to dereference the whole thing, or you will
    		  // assign an address to data[i]
    //		  EEPROM.data[i] = *((char*)(&device) + i);
    	//  }
      }
      EEPROM.write(0, 0);
      EEPROM.commit();

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

  if(minInterval.expired()) {
	  minInterval.set(60000);
	  //minInterval.set(5000);
	  timeClient.update();

	  digitalWrite(LED1_PIN, LOW);



	  String alarm = String(bitRead(devices[2].flags, OUTPUT_BIT) + bitRead(devices[3].flags, OUTPUT_BIT) * 10);

	  String l = String(bitRead(devices[0].flags, OUTPUT_BIT) + bitRead(devices[0].flags, MANUAL_BIT) * 10);
	  String f = String(bitRead(devices[1].flags, OUTPUT_BIT) + bitRead(devices[1].flags, MANUAL_BIT) * 10);
	  String get = GET + "SGRXDOXDL4F6CIGQ" + "&field1=" + alarm +"&field2="+ String(temperature) + "&field3=" + l  + "&field4=" + f;
	  Serial.println(get);
	  HTTPClient http;  //Declare an object of class HTTPClient

	  int httpCode;
	  do {
		  http.begin("http://api.thingspeak.com/talkbacks/18221/commands/execute?api_key=ED57BAC7V0GLPWL2");
		  httpCode = http.GET();                                                                  //Send the request
		  //Serial.println(httpCode);
		  //Serial.println(http.errorToString(httpCode));

		  if (httpCode > 0) { //Check the returning code
			  String payload = http.getString();   //Get the request response payload
			  Serial.println(payload);                     //Pri
			  if(payload == "")
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
		  }
		  else
			  break;
	  } while (true);

	  http.begin(get);  //Specify request destination
	  httpCode = http.GET();                                                                  //Send the request
	  //Serial.println(httpCode);
	  //Serial.println(http.errorToString(httpCode));
	  if (httpCode > 0) { //Check the returning code
		String payload = http.getString();   //Get the request response payload
		Serial.println(payload);                     //Print the response payload
	  }

	      http.end();   //Close connection
	      digitalWrite(LED1_PIN, HIGH);
  }
  if (secInterval.expired()) {
  		secInterval.set(1000);
  		if(ap) {
  			digitalWrite(LED1_PIN, led1);
  			led1 = !led1;
  		}



  		oneWireSensors.requestTemperatures(); // Send the command to get temperatures
  		temperature = oneWireSensors.getTempCByIndex(0);
  		//Serial.println(temperature);

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
			if(i==2) {
				if(bitRead(devices[0].flags, OUTPUT_BIT)) {
					//light
					if(temperature >= devices[2].par1)
						bitSet(devices[2].flags, OUTPUT_BIT);
					if(temperature <= devices[2].par1 - 0.5)
						bitClear(devices[2].flags, OUTPUT_BIT);
				}
				else {
					//dark
					if(temperature >= devices[2].par2)
						bitSet(devices[2].flags, OUTPUT_BIT);
					if(temperature <= devices[2].par2 - 0.5)
						bitClear(devices[2].flags, OUTPUT_BIT);
				}
			}
			if(i==3) {
				if(bitRead(devices[0].flags, OUTPUT_BIT)) {
					//light
					if(temperature <= devices[3].par1)
						bitSet(devices[3].flags, OUTPUT_BIT);
					if(temperature >= devices[3].par1 + 0.5)
						bitClear(devices[3].flags, OUTPUT_BIT);
				}
				else {
					//dark
					if(temperature <= devices[3].par2)
						bitSet(devices[3].flags, OUTPUT_BIT);
					if(temperature >= devices[3].par2 + 0.5)
						bitClear(devices[3].flags, OUTPUT_BIT);
				}

			}

  			//Serial.print(devices[i].flags);
  			//Serial.print('\t');
  			//Serial.print(devices[i].name);
  			//Serial.print('\n');


  		}

		if(bitRead(devices[0].flags, OUTPUT_BIT))
				sendSignal(RFTX_PIN, rf1on);
		else
			sendSignal(RFTX_PIN, rf1off);


		digitalWrite(LED0_PIN, bitRead(devices[2].flags, OUTPUT_BIT) || bitRead(devices[3].flags, OUTPUT_BIT));
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
