#pragma once
#include "Arduino.h"

const int ANIMATION_OFF = 0;
const int ANIMATION_WIPE = 1;
const int ANIMATION_DOUBLEWIPE = 2;
const int ANIMATION_FIRE = 3;
const int ANIMATION_IN = 4;
const int ANIMATION_OUT = 5;
const int ANIMATION_INOUT = 6;
const int ANIMATION_TYPING = 7;
const int ANIMATION_RISING = 8;

const int BRIGHTNESS_LOW = 20;
const int BRIGHTNESS_MID = 128;
const int BRIGHTNESS_MAX = 255;

void readConfig();
void resetConfig();
void saveConfig();

struct GLOBAL {
	static int Animation;
	static int AnimationColorR;
	static int AnimationColorG;
	static int AnimationColorB;
	static int AnimationColorW;
	static int Dimmer;
	static int ColorR;
	static int ColorG;
	static int ColorB;
	static int ColorW;
	static int Brightness;

	static String WlanSSID;
	static String WlanPasswd;
	static String NtpServerIP;
	static bool ItIs;
	static bool ItIsTimeout;
};