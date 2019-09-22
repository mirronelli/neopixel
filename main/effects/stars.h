#ifndef stars_h
#define stars_h

#include "effect.h"

class Stars : Effect
{
public:
	Stars(int pixelCount, int refreshSpeed, int probability, int fadeSpeed, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
	void Run(Pixels* pixels);

private:
	int newStarProbability;
	int fadeSpeed;
	uint8_t	red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
	uint8_t redFadeStep;
	uint8_t greenFadeStep;
	uint8_t blueFadeStep;
	uint8_t whiteFadeStep;
};
#endif