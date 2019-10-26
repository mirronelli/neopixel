#ifndef effectFactory_h
#define effectFactory_h

#include "effect.h"
#include "stars.h"
#include "solid.h"
#include "police.h"
#include "rainbow.h"
#include "snake.h"
#include <string>

class EffectFactory
{
	public:
		static Effect* CreateEffect(std::string nameDefinition, int defaultCount, int defaultDelay);

	private:
		static Effect* CreateSnake(std::string parameters, int defaultCount, int defaultDelay);
		static Effect* CreateStars(std::string parameters, int defaultCount, int defaultDelay);
		static Effect* CreateSolid(std::string parameters, int defaultCount, int defaultDelay);
		static Effect* CreateRainbow(std::string parameters, int defaultCount, int defaultDelay);
		static Effect* CreatePolice(std::string parameters, int defaultCount, int defaultDelay);
};

class ParameterParser
{
	public:
		ParameterParser(std::string);
		int GetNextInt();
		double GetNextDouble();
		std::string GetNextString();
	private:
		size_t from = 0;
		std::string input;
};
#endif
