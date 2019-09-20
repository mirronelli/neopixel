#ifndef effect_h
#define effect_h

#include "../neopixel.h"

class Effect
{
	protected:
		Pixels* pixels;
		int pixelCount;
		int refreshSpeed;

	public:
		Effect(Pixels *pixels, int pixelCount, int refreshSpeed):
			pixels(pixels),
			pixelCount(pixelCount),
			refreshSpeed(refreshSpeed){};

		void WriteAndWait();
};
#endif