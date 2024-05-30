#include "wortuhr_func.h"

#include "config.h"
#include "LedStrip.h"
#include "Logging.h"

#include "Arduino.h"
#include <list>
#include "Animationen.h"


int dimmerOn = 100;
int dimmerOff = 255;

int laststep = -1;
int last_minstep = -2;
int oldYear = 9999;
bool christmastime = false;
String clockText;

std::list<int> clockLetters;

std::list<int> esist_arr =	{ 99,100,102,103,104 };
std::list<int> zwanzig_arr = { 94,93,92,91,90,89,88 };
std::list<int> viertel_arr = { 81,82,83,84,85,86,87 };
std::list<int> prezehn_arr = { 98,97,96,95 };
std::list<int> prefuenf_arr = { 106,107,108,109 };

std::list<int> halb_arr = { 55,56,57,58 };
std::list<int> uhr_arr = { 2,1,0 };
std::list<int> vor_arr = { 70,69,68 };
std::list<int> nach_arr = { 74,73,72,71 };

std::list<int> zwoelf_arr = { 60,61,62,63,64 };
std::list<int> elf_arr = { 32,31,30 };
std::list<int> zehn_arr = { 16,17,18,19 };
std::list<int> neun_arr = { 29,28,27,26 };
std::list<int> acht_arr = { 12,13,14,15 };
std::list<int> sieben_arr = { 49,48,47,46,45,44 };
std::list<int> sechs_arr = { 9,8,7,6,5 };
std::list<int> fuenf_arr = { 40,41,42,43 };
std::list<int> vier_arr = { 25,24,23,22 };
std::list<int> drei_arr = { 34,35,36,37 };
std::list<int> zwei_arr = { 54,53,52,51 };
std::list<int> eins_arr = { 52,51,50,49 };
std::list<int> ein_arr = { 52,51,50 };

void wordClock() {
	wordClock(false);
}

void wordClock(bool force) {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	
	// Feuerwerk bei Jahreswechsel
	if (oldYear < year() && oldYear > 2000) {
		oldYear = year();
		Log("Jahreswechsel von " + String(oldYear) + " nach " + String(year()));
		fireworks();
		return;
	}
	// Weihnachten
	if (month() == 12 && day() >= 24 && day() <= 26) {
		christmastime = true;
	} else {
		christmastime = false;
	}

	int int_minute = minute();

	// Minuten LEDS
	int minstep = int_minute % 5;
	if (last_minstep != minstep || force) {
		switch (minstep) {
		case 1:
			strip.SetPixelColor(110, color);
			strip.SetPixelColor(113, black);
			strip.SetPixelColor(112, black);
			strip.SetPixelColor(111, black);
			if(GLOBAL::ItIsTimeout) turnItIsOff();
			//Log("Minute 1");
			break;
		case 2:
			strip.SetPixelColor(110, color);
			strip.SetPixelColor(113, color);
			strip.SetPixelColor(112, black);
			strip.SetPixelColor(111, black);
			//Log("Minute 2");
			break;
		case 3:
			strip.SetPixelColor(110, color);
			strip.SetPixelColor(113, color);
			strip.SetPixelColor(112, color);
			strip.SetPixelColor(111, black);
			//Log("Minute 3");
			break;
		case 4:
			strip.SetPixelColor(110, color);
			strip.SetPixelColor(113, color);
			strip.SetPixelColor(112, color);
			strip.SetPixelColor(111, color);
			//Log("Minute 4");
			if(christmastime) christmasTree();
			break;
		case 0:
			strip.SetPixelColor(110, black);
			strip.SetPixelColor(113, black);
			strip.SetPixelColor(112, black);
			strip.SetPixelColor(111, black);
			break;
		}
		last_minstep = minstep;
		strip.Show();
	}

	int step = int_minute / 5;

	if (laststep != step || force) {
		//clearLeds();

		clockText = "";
		if (GLOBAL::ItIs) clockText += EsIst();
		switch (step) {
		case 1:
			clockText += pre_fuenf() + nach() + Stunde();
			break;
		case 2:
			clockText += pre_zehn() + nach() + Stunde();
			break;
		case 3:
			clockText += viertel() + nach() + Stunde();
			break;
		case 4:
			clockText += zwanzig() + nach() + Stunde();
			break;
		case 5:
			clockText += pre_fuenf() + vor() + halb() + Stunde(true, false);
			break;
		case 6:
			clockText += halb() + Stunde(true, false);
			break;
		case 7:
			clockText += pre_fuenf() + nach() + halb() + Stunde(true, false);
			break;
		case 8:
			clockText += zwanzig() + vor() + Stunde(true, false);
			break;
		case 9:
			clockText += viertel() + vor() + Stunde(true, false);
			break;
		case 10:
			clockText += pre_zehn() + vor() + Stunde(true, false);
			break;
		case 11:
			clockText += pre_fuenf() + vor() + Stunde(true, false);
			break;
		default:
			clockText += Stunde(false, true) + uhr();
		}
		Log(clockText);
		laststep = step;
		playAnimation();
	}

	strip.SetBrightness(GLOBAL::Brightness);
	//strip.Show();
	oldYear = year();
}

void SetClockWords(std::list<int> letters) {
	for (std::list<int>::const_iterator iterator = letters.begin(), end = letters.end(); iterator != end; ++iterator) {
		clockLetters.push_back(*iterator);
	}
}

void turnItIsOff() {
	for (std::list<int>::const_iterator iterator = esist_arr.begin(), end = esist_arr.end(); iterator != end; ++iterator) {
		SetPixelColor(*iterator, black,true);
		Show();
	}
}

String EsIst() {
	SetClockWords(esist_arr);
	return "Es ist ";
}

String pre_fuenf() {
	SetClockWords(prefuenf_arr);
	return "fünf ";
}

String pre_zehn() {
	SetClockWords(prezehn_arr);
	return "zehn ";
}

String zwanzig() {
	SetClockWords(zwanzig_arr);
	return "zwanzig ";
}

String viertel() {
	SetClockWords(viertel_arr);
	return "viertel ";
}

String nach() {
	SetClockWords(nach_arr);
	return "nach ";
}

String vor() {
	SetClockWords(vor_arr);
	return "vor ";
}

String halb() {
	SetClockWords(halb_arr);
	return "halb ";
}

String zwoelf() {
	SetClockWords(zwoelf_arr);
	return "zwölf ";
}

String zwei() {
	SetClockWords(zwei_arr);
	return "zwei ";
}

String eins() {
	SetClockWords(eins_arr);
	return "eins ";
}

String ein() {
	SetClockWords(ein_arr);
	return "ein ";
}

String sieben() {
	SetClockWords(sieben_arr);
	return "sieben ";
}

String drei() {
	SetClockWords(drei_arr);
	return "drei ";
}

String fuenf() {
	SetClockWords(fuenf_arr);
	return "fünf ";
}

String elf() {
	SetClockWords(elf_arr);
	return "elf ";
}

String neun() {
	SetClockWords(neun_arr);
	return "neun ";
}

String vier() {
	SetClockWords(vier_arr);
	return "vier ";
}

String acht() {
	SetClockWords(acht_arr);
	return "acht ";
}

String zehn() {
	SetClockWords(zehn_arr);
	return "zehn ";
}

String sechs() {
	SetClockWords(sechs_arr);
	return "sechs ";
}

String uhr() {
	SetClockWords(uhr_arr);
	return "Uhr";
}

String Stunde() {
	return Stunde(false, false);
}

String Stunde(bool pluseins, bool voll = false) {
	int int_hour = hourFormat12();
	if (summertime_EU(year(), month(), day(), hour(), 1)) {
		if (int_hour == 12) {
			int_hour = 1;
		} else {
			int_hour = int_hour + 1;
		}
	}

	if (pluseins) {
		int_hour++;
		if (int_hour == 13) int_hour = 1;
	}

	switch (int_hour) {
	case 1:
		if (voll) {
			return ein();
		}
		else {
			return eins();
		}
		break;
	case 2:
		return zwei();
		break;
	case 3:
		return drei();
		break;
	case 4:
		return vier();
		break;
	case 5:
		return fuenf();
		break;
	case 6:
		return sechs();
		break;
	case 7:
		return sieben();
		break;
	case 8:
		return acht();
		break;
	case 9:
		return neun();
		break;
	case 10:
		return zehn();
		break;
	case 11:
		return elf();
		break;
	case 12:
		return zwoelf();
		break;
	default:
		return "";
	}
}

bool toggleDimmer() {
	if (strip.GetBrightness() < 128) {
		Log("Dimmer OFF");
		setBrightness(dimmerOff);
		return false;
	}
	else {
		Log("Dimmer ON");
		setBrightness(dimmerOn);
		return true;
	}
}

void test() {
	clockLetters.clear();
	EsIst();
	noAnimation();
	clockLetters.clear();
	delay(500);
	pre_fuenf();
	noAnimation();
	clockLetters.clear();
	delay(500);
	pre_zehn();
	noAnimation();
	clockLetters.clear();
	delay(500);
	zwanzig();
	noAnimation();
	clockLetters.clear();
	delay(500);
	drei();
	noAnimation();
	clockLetters.clear();
	delay(500);
	viertel();
	noAnimation();
	clockLetters.clear();
	delay(500);
	nach();
	noAnimation();
	clockLetters.clear();
	delay(500);
	vor();
	noAnimation();
	clockLetters.clear();
	delay(500);
	halb();
	noAnimation();
	clockLetters.clear();
	delay(500);
	zwoelf();
	noAnimation();
	clockLetters.clear();
	delay(500);
	zwei();
	noAnimation();
	clockLetters.clear();
	delay(500);
	sieben();
	noAnimation();
	clockLetters.clear();
	delay(500);
	drei();
	noAnimation();
	clockLetters.clear();
	delay(500);
	fuenf();
	noAnimation();
	clockLetters.clear();
	delay(500);
	elf();
	noAnimation();
	clockLetters.clear();
	delay(500);
	neun();
	noAnimation();
	clockLetters.clear();
	delay(500);
	vier();
	noAnimation();
	clockLetters.clear();
	delay(500);
	acht();
	noAnimation();
	clockLetters.clear();
	delay(500);
	zehn();
	noAnimation();
	clockLetters.clear();
	delay(500);
	sechs();
	noAnimation();
	clockLetters.clear();
	delay(500);
	uhr();
	noAnimation();
	clockLetters.clear();
}

boolean summertime_EU(int year, int month, int day, int hour, int tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
	if (month < 3 || month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
	if (month > 3 && month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
	if ((month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7))) || (month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7))))
		return true;
	else
		return false;
}

wortuhr::wortuhr()

{
}


wortuhr::~wortuhr()
{
}
