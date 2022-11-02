#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>
#include <Timer.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "NTPClient.h"
#include "Logging.h"
#include "WebServer.h"
#include "LedStrip.h"
#include "Animationen.h"

Timer t;

const int LED_PIN = LED_BUILTIN;

const IPAddress _STA_ip = IPAddress(192,168,0,1);
const IPAddress _STA_gw = IPAddress(192,168,0,1);
const IPAddress _STA_sn = IPAddress(255,255,255,0);
const char _STA_ssid[] = "wortuhr";

void InitArduinoOTA();
void CheckWifi();
String getStringSplitValue(String data, char separator, int index);

/*************************** Sketch Code ************************************/

void setup() {
	// -------  Serial & PIN Setting
	Serial.begin(74880);
	//Serial.begin(115200);
	pinMode(LED_PIN, OUTPUT);

	readConfig();

	if (GLOBAL::WlanSSID == "" | GLOBAL::WlanPasswd == ""){
		Log("Start WLAN AP");
		WiFi.mode(WIFI_AP);
		WiFi.hostname("wortuhr");
  		WiFi.softAPConfig(_STA_ip, _STA_gw, _STA_sn);
  		WiFi.softAP(_STA_ssid);
		StartWebServer(true);
	} else {

		// -------- WLAN Connection
		Log("Connecting to WLAN");
		WiFi.mode(WIFI_STA);
		wifi_set_sleep_type(NONE_SLEEP_T);
		WiFi.setAutoReconnect(true);
		WiFi.hostname("wortuhr");
		WiFi.begin(GLOBAL::WlanSSID, GLOBAL::WlanPasswd);

		int attempts = 0;
		while (WiFi.status() != WL_CONNECTED && attempts < 200) {
			digitalWrite(LED_PIN, HIGH);
			delay(200);
			digitalWrite(LED_PIN, LOW);
			delay(200);
			Serial.print(".");
			attempts++;
		}
		Serial.println();
		if (WiFi.status() != WL_CONNECTED) {
			GLOBAL::WlanPasswd = "";
			GLOBAL::WlanSSID = "";
			ESP.restart();
		}

		IPAddress HTTPS_ServerIP = WiFi.localIP(); // Obtain the IP of the Server
		Log("Server IP is: " + HTTPS_ServerIP.toString()); // Print the IP to the monitor window
		long rssi = WiFi.RSSI();
		Log("WLAN Signal: " + String(rssi));

		InitArduinoOTA();

		// WebServer Initialisierung
		StartWebServer();

		// Zeitabruf per hour(), minute(), second(), now()
		if (GLOBAL::NtpServerIP == "") {
			setTimeServer(WiFi.gatewayIP());
		} else {
			IPAddress ip;
			ip.fromString(GLOBAL::NtpServerIP);
			setTimeServer(ip);
		}

		updateTime();
		t.every(43200000, updateTime);

		oldYear = year();

		t.every(10000, wordClock);
		t.every(10000, CheckWifi);

		// this resets all the neopixels to an off state
		strip.Begin();
		strip.Show();
	}
}

void loop() {
	WebServer.handleClient();
	ArduinoOTA.handle();
	t.update();
	delay(100);
}



void timeOut() {
	Serial.println(String(hour()) + ":" + String(minute()));
}

void InitArduinoOTA() {
	ArduinoOTA.onStart([]() {

	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR) {
			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			Serial.println("End Failed");
		}
	});
	ArduinoOTA.setHostname("wortuhr");
	ArduinoOTA.begin();
}

String getStringSplitValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void CheckWifi() {
    if (WiFi.status() != WL_CONNECTED) {
        //Log("WiFi disconnected");
        WiFi.reconnect();
    }
}
