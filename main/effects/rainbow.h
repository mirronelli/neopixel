#ifndef rainbow_h
#define rainbow_h

#include "effect.h"

class Rainbow : Effect
{
public:
	Rainbow(int pixelCount, int refreshSpeed);
	void Run(Pixels *pixels);

private:
	enum class Phase
	{
		rising,
		top,
		descending,
		bottom
	};

	void FirstRun(Pixels *pixels);

	int phaseLength;
	Phase redPhase = Phase::top;
	Phase greenPhase = Phase::rising;
	Phase bluePhase = Phase::bottom;
	double phaseStepCoefficient;
	int firstPixel = 0;
	bool firstRun = true;
};
#endif