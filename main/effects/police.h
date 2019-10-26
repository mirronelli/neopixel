#ifndef police_h
#define police_h

#include "effect.h"

class Police : Effect
{
public:
	Police(int pixelCount, int refreshSpeed, uint8_t brightness);
	void Run(Pixels *pixels);

private:
	void FirstRun(Pixels *pixels);
	int firstPixel = 0;
	int halfPixels;
	bool firstRun = true;
	uint8_t brightness = 255;
};
#endif