#ifndef police_h
#define police_h

#include "../neopixel.h"
#include "effect.h"

class Police : Effect
{
	public:
		Police(Pixels *pixels, int pixelCount, int refreshSpeed):
			Effect(pixels, pixelCount, refreshSpeed),
			halfPixels(pixelCount/2){};

		void Run();
		
	private:
		void FirstRun();
		int firstPixel = 0;
		int halfPixels;
		bool firstRun = true;

};
#endif