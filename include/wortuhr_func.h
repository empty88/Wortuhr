#pragma once

#include <TimeLib.h>
#include "Arduino.h"
#include <list>

extern int dimmerOn;
extern int dimmerOff;

extern int oldYear;

bool toggleDimmer();

void wordClock();
void wordClock(bool force);

void test();
boolean summertime_EU(int year, int month, int day, int hour, int tzHours);

extern std::list<int> clockLetters;

String EsIst();
String pre_fuenf();
String pre_zehn();
String zwanzig();
String viertel();
String nach();
String vor();
String halb();
String zwoelf();
String zwei();
String eins();
String ein();
String sieben();
String drei();
String fuenf();
String elf();
String neun();
String vier();
String acht();
String zehn();
String sechs();
String uhr();
String Stunde();
String Stunde(bool pluseins, bool voll);

class wortuhr
{
public:
	wortuhr();
	~wortuhr();
};
