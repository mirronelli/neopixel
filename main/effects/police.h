#ifndef police_h
#define police_h

#include "effect.h"

class Police : Effect
{
public:
	Police(int pixelCount, int refreshSpeed);
	void Run(Pixels *pixels);

private:
	void FirstRun(Pixels *pixels);
	int firstPixel = 0;
	int halfPixels;
	bool firstRun = true;
};
#endif