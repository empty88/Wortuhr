#pragma once

#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>
#include <list>
#include "Logging.h"
#include "PixelChange.h"


#define pixelCount 114            //number of pixels in RGB strip

extern NeoPixelBrightnessBus<NeoGrbwFeature, Neo800KbpsMethod> strip;  //GPIO 0

// make sure to set these panel values to the sizes of yours
//const uint8_t PanelWidth = 11;  // 8 pixel x 8 pixel matrix of leds
//const uint8_t PanelHeight = 10;
//const uint8_t TileWidth = 1;  // laid out in 4 panels x 2 panels mosaic
//const uint8_t TileHeight = 1;

typedef RowMajorAlternating180Layout MyPanelLayout;
typedef ColumnMajor90Layout MyTilesLayout;
extern NeoTiles <MyPanelLayout, MyTilesLayout> tiles;

extern RgbwColor black;
extern RgbwColor red;
extern RgbwColor green;
extern RgbwColor blue;
extern RgbwColor yellow;
extern RgbwColor orange;
extern RgbwColor brown;

extern RgbColor whitergb;
extern RgbwColor white;

extern std::list<PixelChange> changes;

void setBrightness(int value);
void setColor(long value);
void setAnimColor(long value);
void SetPixelColor(int pixel, RgbwColor color);
void SetPixelColor(int pixel, RgbwColor color, bool blend);
void Show();
int GetTop(int pixel);
int GetLeft(int pixel);

void LedTest();
void DumpTopo();

void clearLeds();
void MatrixTest();



class LedStrip
{
public:
	LedStrip();
	~LedStrip();
};
