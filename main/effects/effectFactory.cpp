#include "effectFactory.h"
#include "effect.h"
#include "stars.h"
#include "solid.h"
#include "police.h"
#include "rainbow.h"
#include "snake.h"

#include <string>

using namespace std;

Effect *EffectFactory::CreateEffect(string effectDefinition, int defaultCount, int defaultDelay)
{
	Effect *effect = nullptr;

	size_t separatorPos = effectDefinition.find(':');
	string effectName = effectDefinition.substr(0, separatorPos);
	string effectParameters = separatorPos == string::npos ? "" : effectDefinition.substr(separatorPos + 1, string::npos);

	try
	{
		if (effectName == "snake")
		{
			effect = CreateSnake(effectParameters, defaultCount, defaultDelay);
		}
		else if (effectName == "stars")
		{
			effect = CreateStars(effectParameters, defaultCount, defaultDelay);
		}
		else if (effectName == "solid")
		{
			effect = CreateSolid(effectParameters, defaultCount, defaultDelay);
		}
		else if (effectName == "rainbow")
		{
			effect = CreateRainbow(effectParameters, defaultCount, defaultDelay);
		}
		else if (effectName == "police")
		{
			effect = CreatePolice(effectParameters, defaultCount, defaultDelay);
		}
		else
		{
			printf("unknown effect: \"%s\"\n", effectName.c_str());
			__throw_invalid_argument("unknown effect");
		}
	}
	catch (std::exception&)
	{
		printf("error parsing arguments from \"%s\"\n", effectDefinition.c_str());
		__throw_invalid_argument("invalid arguments for effect");
	}

	return effect;
}

Effect *EffectFactory::CreateSnake(string parameters, int defaultCount, int defaultDelay)
{
	if (parameters == "")
	{
		return (Effect *)new Snake(defaultCount, defaultDelay, 32, 8, 255);
	}
	else 
	{
		ParameterParser parser(parameters);
		return (Effect *) new Snake(
			parser.GetNextInt(), // led count
			parser.GetNextInt(), // frame delay
			parser.GetNextInt(), // snake length
			parser.GetNextInt(), // snake dimed length
			parser.GetNextInt()  // brightness
		);
	}
}

Effect *EffectFactory::CreateStars(string parameters, int defaultCount, int defaultDelay)
{
	if (parameters == "")
	{
		return (Effect *)new Stars(defaultCount, defaultDelay, 99'000, 10, 255, 127, 0, 0);
	}
	else 
	{
		ParameterParser parser(parameters);
		return (Effect *) new Stars (
			parser.GetNextInt(), // led count
			parser.GetNextInt(), // frame delay
			parser.GetNextInt(), // new star probability (if random(100 000) > supplied value a new star occurs on a pixel)
			parser.GetNextInt(), // fade speed
			parser.GetNextInt(), // red
			parser.GetNextInt(), // green
			parser.GetNextInt(), // blue
			parser.GetNextInt()  // white
		);
	}
}

Effect *EffectFactory::CreateSolid(string parameters, int defaultCount, int defaultDelay)
{
	if (parameters == "")
	{
		return (Effect *)new Solid(defaultCount, defaultDelay, 255, 255, 255, 255);
	}
	else 
	{
		ParameterParser parser(parameters);
		return (Effect *) new Solid (
			parser.GetNextInt(), // led count
			parser.GetNextInt(), // frame delay
			parser.GetNextInt(), // red
			parser.GetNextInt(), // green
			parser.GetNextInt(), // blue
			parser.GetNextInt()  // white
		);
	}
}

Effect *EffectFactory::CreateRainbow(string parameters, int defaultCount, int defaultDelay)
{
	if (parameters == "")
	{
		return (Effect *)new Rainbow(defaultCount, defaultDelay, 255);
	}
	else 
	{
		ParameterParser parser(parameters);
		return (Effect *)new Rainbow(
			parser.GetNextInt(), // led count
			parser.GetNextInt(), // frame delay
			parser.GetNextInt()  // brighntness
		); // brightness
	}
}

Effect *EffectFactory::CreatePolice(string parameters, int defaultCount, int defaultDelay)
{
	if (parameters == "")
	{
		return (Effect *)new Police(defaultCount, defaultDelay, 255);
	}
	else 
	{
		ParameterParser parser(parameters);
		return (Effect *) new Police(
			parser.GetNextInt(),  // led count
			parser.GetNextInt(),	  // frame delay
			parser.GetNextInt()	  // brightness
		);
	}
}

ParameterParser::ParameterParser(string input) : input(input){}

int ParameterParser::GetNextInt()
{
	return stoi(GetNextString());
}

double ParameterParser::GetNextDouble()
{
	return stoi(GetNextString());
}

string ParameterParser::GetNextString()
{
	size_t to = input.find(";", from);
	if (to == string::npos)
	{
		to = input.length();
	}

	string result = input.substr(from, to - from);
	printf("found param: \"%s\"\n", result.c_str());
	from = to + 1;

	return result;
}