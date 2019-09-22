#include "stars.h"
#include "math.h"
#include <algorithm>

Stars::Stars(int pixelCount, int refreshSpeed, int probability, int fadeSpeed, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) : 
	Effect(pixelCount, refreshSpeed),
	newStarProbability(probability),
	fadeSpeed(fadeSpeed),
	red(red),
	green(green),
	blue(blue),
	white(white),
	redFadeStep(red / fadeSpeed),
	greenFadeStep(green / fadeSpeed),
	blueFadeStep(blue / fadeSpeed),
	whiteFadeStep(white / fadeSpeed)
{}

void Stars::Run(Pixels* pixels)
{
	for (int i = 0; i < pixelCount; i++)
	{
		Pixel pixel = pixels->GetPixel(i);

		if (rand() % 100'000 > newStarProbability)
		{
			pixel.red = red;
			pixel.green = green;
			pixel.blue = blue;
			pixel.white = white;
		}
		else
		{
			pixel.red	= std::max(0, pixel.red		- redFadeStep);
			pixel.green = std::max(0, pixel.green	- greenFadeStep);
			pixel.blue 	= std::max(0, pixel.blue	- blueFadeStep);
			pixel.white = std::max(0, pixel.white	- whiteFadeStep);
		}

		pixels->SetPixel(i, pixel);
	}
	
	WriteAndWait(pixels);
}