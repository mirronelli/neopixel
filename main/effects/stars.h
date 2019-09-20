#ifndef stars_h
#define stars_h

#include "../neopixel.h"
#include "effect.h"

class Stars : Effect
{
public:
	Stars(Pixels *pixels, int pixelCount, int refreshSpeed, int probability, int fadeSpeed, uint8_t red, uint8_t green, uint8_t blue, uint8_t white):
		Effect(pixels, pixelCount, refreshSpeed),
		newStarProbability(probability),
		fadeSpeed(fadeSpeed),
		red(red),
		green(green),
		blue(blue),
		white(white){}
	void Run();

private:
	int newStarProbability;
	int fadeSpeed;
	uint8_t	red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
};
#endif