#ifndef rainbow_h
#define rainbow_h

#include "effect.h"

class Rainbow : Effect
{
public:
	Rainbow(int pixelCount, int refreshSpeed, uint8_t brightness);
	void Run(Pixels *pixels);

private:
	void FirstRun(Pixels *pixels);

	int phaseLength;
	double phaseStepCoefficient;
	uint8_t brightness;
	int firstPixel = 0;
	bool firstRun = true;
};
#endif