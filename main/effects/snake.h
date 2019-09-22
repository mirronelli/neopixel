#ifndef snake_h
#define snake_h

#include "effect.h"

class Snake : Effect
{
public:
	Snake(int pixelCount, int refreshSpeed, int effectPixelCount, int dimmedCount);
	void Run(Pixels *pixels);

private:
	void HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount);
	int firstPixel = 0;
	int effectPixelCount;
	int dimmedCount;
	int step;
};
#endif