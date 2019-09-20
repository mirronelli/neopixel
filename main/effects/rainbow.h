#ifndef rainbow_h
#define rainbow_h

#include "effect.h"
#include "math.h"

class Rainbow : Effect
{
	public:
		Rainbow(Pixels *pixels, int pixelCount, int refreshSpeed):
			Effect(pixels, pixelCount, refreshSpeed),
			phaseLength(pixelCount/6),
			phaseStepCoefficient(pow(256, 1.0 / phaseLength))
			{};

		void Run();

	private:
		enum class Phase {
			rising,
			top,
			descending,
			bottom
		};

		void FirstRun();

		int phaseLength;
		Phase redPhase = Phase::top;
		Phase greenPhase = Phase::rising;
		Phase bluePhase = Phase::bottom;
		double phaseStepCoefficient;
		int firstPixel = 0;
		bool firstRun = true;
};
#endif