#ifndef police_h
#define police_h

#include "../neopixel.h"
#include "effect.h"

class Police : Effect
{
	private:
		int firstPixel = 0;
		int halfPixels;

	public:
		Police(Pixels *pixels, int pixelCount, int refreshSpeed):
			Effect(pixels, pixelCount, refreshSpeed),
			halfPixels(pixelCount/2){};

		void Run();
};
#endif