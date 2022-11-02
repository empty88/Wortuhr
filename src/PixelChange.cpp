#include "PixelChange.h"
#include "NeoPixelBus.h"


int id;
RgbwColor current;
RgbwColor future;
bool blend;

PixelChange::PixelChange()
{
}

PixelChange::PixelChange(int id1, RgbwColor current1, RgbwColor future1) {
	PixelChange(id1, current1, future1, false);
}
PixelChange::PixelChange(int id1, RgbwColor current1, RgbwColor future1, bool blend1) {
	id = id1;
	current = current1;
	future = future1;
	blend = blend1;
}


PixelChange::~PixelChange()
{
}
