#include "WebServer.h"

#include "config.h"
#include "Animationen.h"
#include "NTPClient.h"
#include "wortuhr_func.h"
#include "LedStrip.h"

ESP8266WebServer WebServer(80);

String onmessage = "&nbsp;";
String offmessage = "&nbsp;";
static String animColor = "#FF0000";
static String color = "#FF0000";

void StartWebServer() {
	StartWebServer(false);
}

void StartWebServer(bool configMode) {
	Serial.println("Start WebServer");

	if (configMode) {
		WebServer.on("/", HandleRootConfigMode);
		WebServer.on("/configWLAN", HandleConfigWLAN);
	} else {
		WebServer.on("/", HandleRoot);
		WebServer.on("/UpdateNTP", HandleUpdateNTP);
		WebServer.on("/getTime", HandleGetTime);
		WebServer.on("/dimmer", HandleToggleDimmer);
		WebServer.on("/configAnim", HandleConfigAnim);
		WebServer.on("/configBrightness", HandleConfigBrightness);
		WebServer.on("/configColor", HandleConfigColor);
		WebServer.on("/Logs", HandleLogs);

		WebServer.on("/brightness", HandleBrightness);
		WebServer.on("/color", HandleColor);
		WebServer.on("/animColor", HandleAnimColor);
		WebServer.on("/saveAnim", HandleSaveAnim);
		WebServer.on("/saveColor", HandleSaveColor);
		WebServer.on("/saveBrightness", HandleSaveBrightness);
		WebServer.on("/getLogs", HandleGetLogs);

		WebServer.on("/test", HandleTest);
		WebServer.on("/refresh", HandleRefresh);
		WebServer.on("/clear", HandleClear);
		WebServer.on("/anim", HandleAnimStart);
		WebServer.on("/matrix", HandleMatrixTest);
		WebServer.on("/restart", HandleRestart);
		WebServer.on("/reset", HandleReset);
		WebServer.on("/fireworks", HandleFireworks);
		WebServer.on("/christmas", HandleChristmas);
	}
	WebServer.begin();

}



void ShowPage(String content, String script, String style) {
	String page = FPSTR(HTTP_HEADER);
	page += script;
	page += FPSTR(HTTP_SCRIPT_END);
	page += style;
	page += FPSTR(HTTP_CONTAINER);

	// replacements
	content.replace("{menu}", FPSTR(HTTP_MENU));
	content.replace("{onmessage}", onmessage);
	content.replace("{offmessage}", offmessage);
	content.replace("{brightness}", String(GLOBAL::Brightness));
	content.replace("{color}", color);
	content.replace("{animColor}", animColor);
	onmessage = "&nbsp;";
	offmessage = "&nbsp;";
	page += content;
	page += FPSTR(HTTP_END);
	WebServer.send(200, "text/html", page);
}

void HandleRoot() {
	Serial.println("HandleRoot");
	String content = FPSTR(HTTP_MAIN);
	content += "{menu}";

	ShowPage(content, FPSTR(HTTP_MAIN_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleRootConfigMode() {
	Serial.println("HandleRootConfigMode");
	String content = FPSTR(HTTP_MAIN_CONFIG_MODE);

	ShowPage(content, "", FPSTR(HTTP_STYLE));
}

void HandleConfigWLAN() {
	String ssid = WebServer.arg("ssid");
	String passwd = WebServer.arg("passwd");
	String timesvr = WebServer.arg("timesvr");
	GLOBAL::WlanSSID = ssid;
	GLOBAL::WlanPasswd = passwd;
	GLOBAL::NtpServerIP = timesvr;
	saveConfig();
	ESP.restart();
}

void HandleUpdateNTP() {
	Serial.println("HandleUpdateNTP");
	updateTime();
	wordClock();
	onmessage = "Uhrzeit wurde aktualisiert!";

	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleToggleDimmer() {
	Serial.println("HandleToggleDimmer");
	if (toggleDimmer()) {
		onmessage = "Dimmer eingeschaltet";
		GLOBAL::Dimmer = 1;
	}
	else {
		offmessage = "Dimmer ausgeschaltet";
		GLOBAL::Dimmer = 0;
	}
	
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleConfigAnim() {
	Serial.println("HandleConfigAnim");
	String content = FPSTR(HTTP_MENU_BACK);
	content += FPSTR(HTTP_ANIM_CONFIG);

	for (unsigned int i = 0; i < sizeof(ANIM_VALUES); i++) {
		if (i == (unsigned int) GLOBAL::Animation) {
			content.replace("{" + String(i) + "_selected}", "selected");
		} else {
			content.replace("{" + String(i) + "_selected}", "");
		}
	}

	char hexout[8];
	if(GLOBAL::AnimationColorR == 0 && GLOBAL::AnimationColorG == 0 && GLOBAL::AnimationColorB == 0 && GLOBAL::AnimationColorW == 255) {
		sprintf(hexout,"#%02x%02x%02x",255,255,255);
	} else {
		sprintf(hexout,"#%02x%02x%02x",GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB);
	}

	animColor = String(hexout);

	ShowPage(content, FPSTR(HTTP_ANIM_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleConfigBrightness() {
	Serial.println("HandleConfigBrightness");
	String content = FPSTR(HTTP_MENU_BACK);
	content += FPSTR(HTTP_BRIGHTNESS_CONFIG);

	ShowPage(content, FPSTR(HTTP_BRIGHTNESS_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleConfigColor() {
	Serial.println("HandleConfigColor");
	String content = FPSTR(HTTP_MENU_BACK);
	content += FPSTR(HTTP_COLOR_CONFIG);

	char hexout[8];
	if(GLOBAL::ColorR == 0 && GLOBAL::ColorG == 0 && GLOBAL::ColorB == 0 && GLOBAL::ColorW == 255) {
		sprintf(hexout,"#%02x%02x%02x",255,255,255);
	} else {
		sprintf(hexout,"#%02x%02x%02x",GLOBAL::ColorR,GLOBAL::ColorG,GLOBAL::ColorB);
	}
	
	color = String(hexout);

	ShowPage(content, FPSTR(HTTP_COLOR_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleSaveAnim() {
	Serial.println("HandleSaveAnim");

	String colorstr = WebServer.arg("animColorPicker").substring(1);
	int result;
	result = strtoul(colorstr.c_str(), NULL, 16);
	setAnimColor(result);
	Log("Anim Color: " + colorstr);

	GLOBAL::Animation = WebServer.arg("anim").toInt();
	Log("Animation saved: " + String(GLOBAL::Animation));

	onmessage = "Einstellungen gespeichert";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
	saveConfig();
}

void HandleSaveColor() {
	Serial.println("HandleSaveColor");

	String colorstr = WebServer.arg("colorPicker").substring(1);
	int result;
	result = strtoul(colorstr.c_str(), NULL, 16);
	setColor(result);
	Log("Color saved: " + colorstr);
	
	onmessage = "Einstellungen gespeichert";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
	saveConfig();
}

void HandleSaveBrightness() {
	Serial.println("HandleSaveBrightness");

	int brightness = WebServer.arg("brightness").toInt();
	GLOBAL::Brightness = brightness;

	Log("Brightness saved: " + String(brightness));

	onmessage = "Einstellungen gespeichert";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
	saveConfig();
}

void HandleColor() {
	Serial.println("HandleColor");
	String colorstr = WebServer.arg("value").substring(1);
	int result;
	result = strtoul(colorstr.c_str(), NULL, 16);
	setColor(result);
	Log("Farbe: " + colorstr);
	onmessage = "Einstellungen gespeichert";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleAnimColor() {
	Serial.println("HandleAnimColor");
	String colorstr = WebServer.arg("value").substring(1);
	int result;
	result = strtoul(colorstr.c_str(), NULL, 16);
	setAnimColor(result);
	Log("Anim Farbe: " + colorstr);
	onmessage = "Einstellungen gespeichert";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleBrightness() {
	Log("HandleBrightness");

	setBrightness(WebServer.arg("value").toInt());

	onmessage = "Einstellungen gespeichert";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleLogs() {
	Serial.println("HandleGetLogs");
	String content = "<textarea readonly id=\"logs\" cols=\"45\" wrap=\"off\"></textarea>";
	content += FPSTR(HTTP_MENU_BACK);

	ShowPage(content, FPSTR(HTTP_LOGS_SCRIPT), FPSTR(HTTP_STYLE));
}

void HandleTest() {
	Serial.println("HandleTest");

	onmessage = "Test wird ausgeführt!";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
	LedTest();
	test();
}

void HandleRefresh() {
	Log("HandleRefresh");

	onmessage = "Refresh wird ausgeführt!";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
	wordClock(true);
}

void HandleClear() {
	clearLeds();
	strip.Show();
	onmessage = "Anzeige zurückgesetzt";
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleAnimStart() {
	playAnimation();
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}
void HandleMatrixTest() {
	MatrixTest();
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleReset() {
	resetConfig();
	ESP.restart();
}

void HandleFireworks() {
	fireworks();
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleChristmas() {
	christmasTree();
	WebServer.sendHeader("Location", String("/"), true);
	WebServer.send(302, "text/plain", "");
}

void HandleGetLogs() {
	WebServer.send(200, "text/html", GetLogs());
}

void HandleGetTime() {
	char timestr[21];
	int int_hour = hour();
	if (summertime_EU(year(), month(), day(), hour(), 1)) {
		if (int_hour == 24) {
			int_hour = 0;
		}
		else {
			int_hour = int_hour + 1;
		}
	}
	sprintf(timestr, "%02d.%02d.%02d - %02d:%02d:%02d", day(), month(), year(), int_hour, minute(), second());

	WebServer.send(200, "text/html", timestr);
}

void HandleRestart() {
  ESP.restart();
}

WebServer::WebServer()
{
	
}


WebServer::~WebServer()
{
}
