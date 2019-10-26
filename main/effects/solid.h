#ifndef solid_h
#define solid_h

#include "effect.h"

class Solid : Effect
{
public:
	Solid(int pixelCount, int refreshSpeed, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
	void Run(Pixels* pixels);

private:
	uint8_t	red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
};
#endif