#ifndef snake_h
#define snake_h

#include "effect.h"

class Snake : Effect
{
public:
	Snake(int pixelCount, int refreshSpeed, int effectPixelCount, int dimmedCount, uint8_t brightness);
	void Run(Pixels *pixels);

private:
	void HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount, uint8_t brightness);
	int firstPixel = 0;
	int effectPixelCount;
	int dimmedCount;
	int step;
	uint8_t brightness;
};
#endif