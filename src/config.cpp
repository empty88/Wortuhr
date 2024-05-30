#include "config.h"
#include <LittleFS.h>

#include "Logging.h"

String removeTrailingCR(String input);

int GLOBAL::Animation = ANIMATION_OFF;
int GLOBAL::AnimationColorR = 0;
int GLOBAL::AnimationColorG = 0;
int GLOBAL::AnimationColorB = 0;
int GLOBAL::AnimationColorW = 0;
int GLOBAL::ColorR = 0;
int GLOBAL::ColorG = 0;
int GLOBAL::ColorB = 0;
int GLOBAL::ColorW = 255;
int GLOBAL::Dimmer = 0;
int GLOBAL::Brightness = BRIGHTNESS_MID;
String GLOBAL::WlanSSID = "";
String GLOBAL::WlanPasswd = "";
String GLOBAL::NtpServerIP = "";
bool GLOBAL::ItIs = true;
bool GLOBAL::ItIsTimeout = true;

void readConfig() {
	LittleFS.begin();
	File configFile = LittleFS.open("_configFile", "r");
	if (configFile) {
		//Log("lese Config-Datei...\n" + configFile.readString());
		configFile.seek(0);
		GLOBAL::Animation = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::AnimationColorR = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
        GLOBAL::AnimationColorG = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
        GLOBAL::AnimationColorB = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
        GLOBAL::AnimationColorW = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::ColorR = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::ColorG = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::ColorB = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::ColorW = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
        GLOBAL::Dimmer = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::Brightness = removeTrailingCR(configFile.readStringUntil('\n')).toInt();
		GLOBAL::WlanSSID = removeTrailingCR(configFile.readStringUntil('\n'));
		GLOBAL::WlanPasswd= removeTrailingCR(configFile.readStringUntil('\n'));
		GLOBAL::NtpServerIP= removeTrailingCR(configFile.readStringUntil('\n'));
		GLOBAL::ItIs= removeTrailingCR(configFile.readStringUntil('\n'));
		GLOBAL::ItIsTimeout= removeTrailingCR(configFile.readStringUntil('\n'));
		configFile.close();
	}
	else {
		Log("Config-Datei nicht vorhanden");
		resetConfig();
	}
	
}

void saveConfig() {
	Log("schreibe Config-Datei...");
	File configFile = LittleFS.open("_configFile", "w");
	configFile.println(GLOBAL::Animation); //Animation
    configFile.println(GLOBAL::AnimationColorR); //AnimationColorR
    configFile.println(GLOBAL::AnimationColorG); //AnimationColorG
    configFile.println(GLOBAL::AnimationColorB); //AnimationColorB
    configFile.println(GLOBAL::AnimationColorW); //AnimationColorW
	configFile.println(GLOBAL::ColorR); //ColorR
	configFile.println(GLOBAL::ColorG); //ColorG
	configFile.println(GLOBAL::ColorB); //ColorB
	configFile.println(GLOBAL::ColorW); //ColorW
    configFile.println(GLOBAL::Dimmer); //Dimmer
	configFile.println(GLOBAL::Brightness); //Brightness
	configFile.println(GLOBAL::WlanSSID); //WLAN SSID
	configFile.println(GLOBAL::WlanPasswd); //WLAN Password
	configFile.println(GLOBAL::NtpServerIP); //NTP Server
	configFile.println(GLOBAL::ItIs); //Es ist...
	configFile.println(GLOBAL::ItIsTimeout); //Es ist timeout
	configFile.close();
	configFile = LittleFS.open("_configFile", "r");
	//Log(configFile.readString());
	configFile.close();
}

void resetConfig() {
	Log("erstelle Default-Settings");
	File configFile = LittleFS.open("_configFile", "w");
	configFile.println("0"); //Animation
	configFile.println("0"); //AnimationColorR
	configFile.println("0"); //AnimationColorG
	configFile.println("0"); //AnimationColorB
	configFile.println("255"); //AnimationColorW
	configFile.println("0"); //ColorR
	configFile.println("0"); //ColorG
	configFile.println("0"); //ColorB
	configFile.println("255"); //ColorW
	configFile.println("0"); //Dimmer
	configFile.println("128"); //Brightness
	configFile.println(""); //WLAN SSID
	configFile.println(""); //WLAN Password
	configFile.println(""); //NTP Server
	configFile.println(true); //Es ist...
	configFile.println(true); //Es ist... timeout
	configFile.close();
}

String removeTrailingCR(String input)
{
	if (!input)
		return String();
	if (input.charAt(input.length() - 1) == 0x0d)
	{
		input.remove(input.length() - 1);
	}
	return input;
}