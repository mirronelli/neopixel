#ifndef snake_h
#define snake_h

#include "../neopixel.h"

class Snake
{
	public:
		static void Run(Pixels *pixels, int pixelCount, int effectPixelCount, int dimmedCount);

	private:
		static void HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount);
};
#endif