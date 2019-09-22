#ifndef effect_h
#define effect_h

#include "../neopixel.h"

class Effect
{
	protected:
		int pixelCount;
		int refreshSpeed;

	public:
		Effect(int pixelCount, int refreshSpeed):
			pixelCount(pixelCount),
			refreshSpeed(refreshSpeed){};

		virtual ~Effect(){};
		virtual void Run(Pixels	*pixels);
		void WriteAndWait(Pixels *pixels);
};
#endif