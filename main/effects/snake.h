#ifndef snake_h
#define snake_h

#include "effect.h"

class Snake : Effect
{
public:
	Snake(int pixelCount, int refreshSpeed, int effectPixelCount, int dimmedCount, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
	void Run(Pixels *pixels);

private:
	void HalfSnake(Pixels *pixels, int firstPixel);
	int firstPixel = 0;
	int effectPixelCount;
	int dimmedCount;
	int step;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
};
#endif