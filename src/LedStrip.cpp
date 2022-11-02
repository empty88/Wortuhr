#include "LedStrip.h"
#include <list>

#include "config.h"

#include "Animationen.h"
#include "PixelChange.h"

#define pixelCount 114            //number of pixels in RGB strip

NeoPixelBrightnessBus<NeoGrbwFeature, Neo800KbpsMethod> strip(pixelCount, RX);  //GPIO 0

// make sure to set these panel values to the sizes of yours
const uint8_t PanelWidth = 11;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 10;
const uint8_t TileWidth = 1;  // laid out in 4 panels x 2 panels mosaic
const uint8_t TileHeight = 1;

NeoTiles <MyPanelLayout, MyTilesLayout> tiles(PanelWidth, PanelHeight, TileWidth, TileHeight);

RgbwColor black(0);
RgbwColor red(128, 0, 0, 0);
RgbwColor green(0, 128, 0, 0);
RgbwColor blue(0, 0, 128, 0);
RgbwColor yellow(255,255,0);
RgbwColor orange(255,128,0);
RgbwColor brown(139,69,19);

RgbColor whitergb(255, 255, 255);
RgbwColor white(255);

std::list<PixelChange> changes;

void setBrightness(int value) {
	Log("Helligkeit: " + String(value));
	GLOBAL::Brightness = value;
	strip.SetBrightness(GLOBAL::Brightness);
	strip.Show();
}

void setColor(long value) {
	Log("Farbe: " + String(value));
	Log("Farbe: " + value);
	if (value == 16777215) {
		GLOBAL::ColorR = 0;
		GLOBAL::ColorG = 0;
		GLOBAL::ColorB = 0;
		GLOBAL::ColorW = 255;
	}
	else {
		RgbColor newColor = RgbColor(HtmlColor(value));
		GLOBAL::ColorR = newColor.R;
		GLOBAL::ColorG = newColor.G;
		GLOBAL::ColorB = newColor.B;
		GLOBAL::ColorW = 0;
	}
	
	for (int i = 0; i < pixelCount; i++) {
		if (strip.GetPixelColor(i) != black) {
			strip.SetPixelColor(i, RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW));
		}
	}
	strip.Show();
}

void setAnimColor(long value) {
	Log("Farbe: " + String(value));
	Log("Farbe: " + value);
	if (value == 16777215) {
		GLOBAL::AnimationColorR = 0;
		GLOBAL::AnimationColorG = 0;
		GLOBAL::AnimationColorB = 0;
		GLOBAL::AnimationColorW = 255;
	}
	else {
		RgbColor newColor = RgbColor(HtmlColor(value));
		GLOBAL::AnimationColorR = newColor.R;
		GLOBAL::AnimationColorG = newColor.G;
		GLOBAL::AnimationColorB = newColor.B;
		GLOBAL::AnimationColorW = 0;
	}
}


void SetPixelColor(int pixel, RgbwColor color) {
	SetPixelColor(pixel, color, false);
}

void SetPixelColor(int pixel, RgbwColor color, bool blend) {
	bool found = false;
	for (std::list<PixelChange>::iterator iterator = changes.begin(), end = changes.end(); iterator != end; ++iterator) {
		if (iterator->id == pixel) {
			iterator->current = strip.GetPixelColor(pixel);
			//iterator->futureChanged = iterator->future != color;
			iterator->future = color;
			iterator->blend = blend;
			found = true;
			break;
		}
	}

	if (!found) {
		changes.push_back(PixelChange(pixel, strip.GetPixelColor(pixel), color, blend));
	}
}

void Show() {
	bool blendRequired = false;
	for (PixelChange item : changes) {
		if (item.blend) {
			blendRequired = true;
			break;
		} else {
			strip.SetPixelColor(item.id, item.future);
		}
	}
	strip.Show();
	if (blendRequired) {
		for (float blend = 0.0; blend <= 1.0; blend += 0.25) {
			for (PixelChange item : changes) {
				if (item.blend) {
					if (item.futureChanged) item.current = strip.GetPixelColor(item.id);
					RgbwColor calcColor = RgbwColor::LinearBlend(item.current, item.future, blend);
					//Log("Rot: " + String(calcColor.R) + " , Grün: " + String(calcColor.G) + ", Blau: " + String(calcColor.B));
					strip.SetPixelColor(item.id, calcColor);
					
				} else {
					strip.SetPixelColor(item.id, item.future);
				}
				delay(1);
			}
			strip.Show();
			delay(20);
		}
	}
	changes.clear();
}

void clearLeds() {
	strip.ClearTo(RgbColor(0, 0, 0));
}

void MatrixTest() {

	for (int x = 0; x <= 10; x++) {
		for (int y = 9; y >= 0; y--) {
			strip.SetPixelColor(tiles.Map(0, y), green);
			strip.Show();
		}

		strip.SetPixelColor(tiles.Map(x, 0), green);
		strip.Show();
	}
	for (int x = 10; x >= 0; x--) {
		for (int y = 0; y <= 9; y++) {
			strip.SetPixelColor(tiles.Map(10, y), green);
			strip.Show();
		}
		strip.SetPixelColor(tiles.Map(x, 9), green);
		strip.Show();
	}


}

void LedTest() {
	int brightness = strip.GetBrightness();
	strip.SetBrightness(20);
	strip.ClearTo(black);
	for (int x = 0; x < pixelCount; x++) {
		Serial.println("LED-Test: " + String(x));
		strip.SetPixelColor(x, RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW));
		strip.Show();
		delay(100);
	}
	strip.SetBrightness(brightness);
}

void DumpTopo()
{
	Serial.println();

	Serial.print("\t\t");
	for (int x = 0; x < tiles.getWidth(); x++)
	{
		Serial.print(x);
		Serial.print("\t");
	}
	Serial.println();

	Serial.print("\t---");
	for (int x = 0; x < tiles.getWidth(); x++)
	{
		Serial.print("--------");
	}
	Serial.println();

	for (int y = 0; y < tiles.getHeight(); y++)
	{
		Serial.print("  ");
		Serial.print(y);
		Serial.print("\t|\t");

		for (int x = 0; x < tiles.getWidth(); x++)
		{
			NeoTopologyHint hint = tiles.TopologyHint(x, y);

			Serial.print(tiles.Map(x, y));
			if (hint == NeoTopologyHint_FirstOnPanel)
			{
				Serial.print("<");
			}
			else if (hint == NeoTopologyHint_LastOnPanel)
			{
				Serial.print(">");
			}
			Serial.print("\t");
		}
		Serial.println();
	}
}

int GetTop(int pixel) {
	return PanelHeight - (int)(pixel / PanelWidth);
}

int GetLeft(int pixel) {
	if (GetTop(pixel) % 2 != 0) {
		return (pixel % PanelWidth) + 1;
	} else {
		return (11 - (pixel % PanelWidth));
	}
}


LedStrip::LedStrip()
{
}


LedStrip::~LedStrip()
{
}
