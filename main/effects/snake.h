#ifndef snake_h
#define snake_h

#include "../neopixel.h"
#include "effect.h"

class Snake : Effect
{
	private:
		void HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount);
		int firstPixel = 0;
		int effectPixelCount;
		int dimmedCount;
		int step;

	public:
		Snake(Pixels *pixels, int pixelCount, int refreshSpeed, int effectPixelCount, int dimmedCount) : 
			Effect(pixels, pixelCount, refreshSpeed),
			effectPixelCount(effectPixelCount),
			dimmedCount(dimmedCount){};

		void Run();
};
#endif