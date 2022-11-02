#pragma once
#include <NeoPixelBus.h>

class PixelChange
{
public:
	PixelChange();
	PixelChange(int id1, RgbwColor current1, RgbwColor future1);
	PixelChange(int id1, RgbwColor current1, RgbwColor future1, bool blend1);
	~PixelChange();
	int id;
	RgbwColor current;
	RgbwColor future;
	bool futureChanged;
	bool blend;
};
