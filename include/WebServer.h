#include "Arduino.h"
#include <TimeLib.h>

#include <ESP8266WebServer.h>

#include "wortuhr_func.h"


extern ESP8266WebServer WebServer;

const char HTTP_HEADER[] PROGMEM =
"<!DOCTYPE html><html lang=\"de\" class=\"\">\n"
"<head>\n"
"<meta charset='utf-8'>\n"
"<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>\n"
"<title>Wortuhr - {v}</title>\n";

const char HTTP_MAIN_SCRIPT[] PROGMEM =
"<script>\n"
"var r;\n"
"function init() {\n"
"clock();\n"
"setTimeout(message, 5000);\n"
"};\n"
"function clock() {\n"
"r = new XMLHttpRequest();\n"
"r.onreadystatechange=function(){\n"
"if(r.readyState==4&&r.status==200){\n"
"document.getElementById(\"time\").innerHTML=r.responseText;\n"
"};\n"
"};\n"
"r.open(\"GET\", \"getTime\");r.send();\n"
"setTimeout(clock ,1000);\n"
"}\n"
"function message() {\n"
"document.getElementById(\"onmessage\").innerHTML=\"&nbsp;\";\n"
"document.getElementById(\"offmessage\").innerHTML=\"&nbsp;\";\n"
"};\n";

const char HTTP_LOGS_SCRIPT[] PROGMEM =
"<script>\n"
"var r;\n"
"function init() {\n"
"	logs();\n"
"};\n"
"function logs() {\n"
"	r = new XMLHttpRequest();\n"
"	r.onreadystatechange=function(){\n"
"	if(r.readyState==4&&r.status==200){\n"
"		l = document.getElementById(\"logs\");\n"
"		l.value=r.responseText;\n"
"		//l.scrollTop=99999;\n"
"		l.scrollTop;\n"
"	};\n"
"};\n"
"r.open(\"GET\", \"getLogs\");r.send();\n"
"setTimeout(logs ,2000);\n"
"};\n";

const char HTTP_BRIGHTNESS_SCRIPT[] PROGMEM =
"<script>\n"
"var slider;\n"
"var output;\n"
"function init() {\n"
"	slider = document.getElementById(\"brightness\");\n"
"	output = document.getElementById(\"range\");\n"
"	output.innerHTML = slider.value;\n"
"}\n"
"function slide(value) {\n"
"	output.innerHTML = value;\n"
"	r = new XMLHttpRequest();\n"
"	r.open(\"GET\", \"brightness?value=\" + value, true);r.send();\n"
"}\n";

const char HTTP_COLOR_SCRIPT[] PROGMEM =
"<script>\n"
"var picker;\n"
"var output;\n"
"function init() {\n"
"	picker = document.getElementById(\"colorPicker\");\n"
"	output = document.getElementById(\"color\");\n"
"	output.innerHTML = picker.value;\n"
"};\n"
"function colorpick(value) {\n"
"	output.innerHTML = value;\n"
"	r = new XMLHttpRequest();\n"
"	r.open(\"GET\", \"color?value=\" + encodeURIComponent(value), true);r.send();\n"
"};\n";

const char HTTP_ANIM_SCRIPT[] PROGMEM =
"<script>\n"
"var picker;\n"
"var output;\n"
"function init() {\n"
"	picker = document.getElementById(\"animColorPicker\");\n"
"	output = document.getElementById(\"animColor\");\n"
"	output.innerHTML = picker.value;\n"
"};\n"
"function colorpick(value) {\n"
"	output.innerHTML = value;\n"
"	r = new XMLHttpRequest();\n"
"	r.open(\"GET\", \"animColor?value=\" + encodeURIComponent(value), true);r.send();\n"
"};\n";


const char HTTP_HEAD_RELOAD[] PROGMEM =
"setTimeout(function(){location.href='.';},4000);\n";

const char HTTP_SCRIPT_END[] PROGMEM =
"</script>\n";

const char HTTP_STYLE[] PROGMEM =
"<style>\n"
"body {\n"
"	font-family: verdana;\n"
"	text-align: center;\n"
"}\n"
"button {\n"
"	border: 0px;\n"
"	border-radius: 0.3rem;\n"
"	background-color: #df335c;\n"
"	color: #fff;\n"
"	line-height: 2.4rem;\n"
"	font-size: 1.2rem;\n"
"	width: 100%;\n"
"	-webkit-transition-duration: 0.4s;\n"
"	transition-duration: 0.4s;\n"
"	cursor: pointer;\n"
"}\n"
"button:hover {\n"
"	background-color:#cf133c;\n"
"}\n"
"div, select, input {\n"
"	padding: 5px;\n"
"	font-size: 1em;\n"
"}\n"
"#onmessage, #offmessage {\n"
"	font-size: 1.4em;\n"
"	font-weight: bold;\n"
"}\n"
"select {\n"
"	width: 100%;\n"
"}\n"
"textarea {\n"
"	resize: none;\n"
"	width: 98%;\n"
"	height: 300px;\n"
"}\n"
".slider {\n"
"	width: 100%;\n"
"}\n"
"</style>\n"
"</head>\n";

const char HTTP_CONTAINER[] PROGMEM =
"<body onload=\"init()\">\n"
"<div style=\"text-align: left; display: inline-block;min-width: 340px;\">\n"
"<div style=\"text-align:center;\"><h2>Wortuhr</h2><h3 id=\"time\"></h3></div>\n";

const char HTTP_MAIN[] PROGMEM =
"<div style=\"text-align:center;\">\n"
"<span id=\"onmessage\" style=\"color: #99ff99;\">{onmessage}</span>\n"
"<span id=\"offmessage\" style=\"color: #ff9999;\">{offmessage}</span>\n"
"</div>\n"
"<br>\n";

const char HTTP_MAIN_CONFIG_MODE[] PROGMEM =
"<form action=\"configWLAN\" method=\"get\">\n"
"<div width=\"50px\">WLAN SSID:</div><input type=\"text\" id=\"ssid\" name=\"ssid\" required size=\"10\"><br />\n"
"<div width=\"100px\">WLAN Passwort:</div><input type=\"text\" id=\"passwd\" name=\"passwd\" required size=\"10\"><br />\n"
"<div width=\"100px\">Zeitserver:</div><input type=\"text\" id=\"timesvr\" name=\"timesvr\" size=\"10\"> wenn leer => Gateway<br />\n"
"<br />\n"
"<button>Übernehmen</button>\n"
"</form>\n";

const char HTTP_MENU[] PROGMEM =
"<form action=\"UpdateNTP\" method=\"get\"><button>NTP-Zeit Sync</button></form>\n<br>\n"
"<form action=\"dimmer\" method=\"get\"><button>Dimmer umschalten</button></form>\n<br>\n"
"<form action=\"configAnim\" method=\"get\"><button>Animation</button></form>\n<br>\n"
"<form action=\"configBrightness\" method=\"get\"><button>Helligkeit</button></form>\n<br>\n"
"<form action=\"configColor\" method=\"get\"><button>Farbe</button></form>\n<br>\n"
"<form action=\"Logs\" method=\"get\"><button>Logs</button></form>\n<br>\n"
"<form action=\"refresh\" method=\"get\"><button>Refresh</button></form>\n<br>\n"
"<form action=\"restart\" method=\"get\"><button>Neustart</button></form>\n<br>\n";

const char HTTP_MENU_BACK[] PROGMEM =
"<form action=\"/\" method=\"get\"><button>Zurück</button></form>\n<br>\n";

const char HTTP_ANIM_CONFIG[] PROGMEM =
"<form action=\"saveAnim\" method=\"get\">\n"
"<select name=\"anim\">\n"
"<option {0_selected} value=\"0\">Keine</option>\n"
"<option {1_selected} value=\"1\">Wischen</option>\n"
"<option {2_selected} value=\"2\">Doppel-Wischen</option>\n"
"<option {3_selected} value=\"3\">Feuer</option>\n"
"<option {4_selected} value=\"4\">Implosion</option>\n"
"<option {5_selected} value=\"5\">Explosion</option>\n"
"<option {6_selected} value=\"6\">Impl-Expl</option>\n"
"<option {7_selected} value=\"7\">Schreiben</option>\n"
"<option {8_selected} value=\"8\">Rising Letters</option>\n"
"</select>\n"
"<br>\n"
"<input type=\"color\" value=\"{animColor}\" class=\"colorpicker\" id=\"animColorPicker\" name=\"animColorPicker\" onchange=\"colorpick(this.value)\">\n"
"<div id=\"animColor\">\n"
"</div>\n"
"<br>\n"
"<button type=\"submit\">Speichern</button>\n"
"</form>\n";

const char HTTP_BRIGHTNESS_CONFIG[] PROGMEM =
"<form action=\"saveBrightness\" method=\"get\">\n"
"<div class = \"slidecontainer\">\n"
"<input type=\"range\" min=\"1\" max=\"255\" value=\"{brightness}\" class=\"slider\" name=\"brightness\" onchange=\"slide(this.value)\">\n"
"<br />\n"
"<div id=\"range\">\n"
"</div>\n"
"</div>\n"
"<br>\n"
"<button type=\"submit\">Speichern</button>\n"
"</form>\n";

const char HTTP_COLOR_CONFIG[] PROGMEM =
"<form action=\"saveColor\" method=\"get\">\n"
"<div class = \"colorcontainer\">\n"
"<input type=\"color\" value=\"{color}\" class=\"colorpicker\" id=\"colorPicker\" name=\"colorPicker\" onchange=\"colorpick(this.value)\">\n"
"<br />\n"
"<div id=\"color\">\n"
"</div>\n"
"</div>\n"
"<br>\n"
"<button type=\"submit\">Speichern</button>\n"
"</form>\n";

const char HTTP_END[] PROGMEM =
"<br/>\n"
"</div>\n"
"</body>\n"
"</html>\n";

extern String onmessage;
extern String offmessage;

void HandleRoot();
void HandleRootConfigMode();
void HandleConfigWLAN();
void HandleUpdateNTP();

void HandleLogs();

void HandleToggleDimmer();
void HandleConfigAnim();
void HandleConfigBrightness();
void HandleConfigColor();

void HandleSaveAnim();
void HandleSaveColor();
void HandleSaveBrightness();
void HandleGetLogs();
void HandleGetTime();
void HandleBrightness();
void HandleColor();
void HandleAnimColor();

void HandleTest();
void HandleRefresh();
void HandleAnimStart();
void HandleClear();
void HandleMatrixTest();
void HandleFireworks();
void HandleChristmas();

void HandleRestart();
void HandleReset();

void StartWebServer();
void StartWebServer(bool configMode);


String MainMenu();

class WebServer
{
public:
	WebServer();
	~WebServer();
};
