#include "snake.h"
#include <algorithm>

Snake::Snake(int pixelCount, int refreshSpeed, int effectPixelCount, int dimmedCount, uint8_t brightness) : 
	Effect(pixelCount, refreshSpeed),
	effectPixelCount(effectPixelCount),
	dimmedCount(dimmedCount),
	brightness(brightness){};

void Snake::Run(Pixels* pixels)
{
	HalfSnake(pixels, firstPixel, effectPixelCount, dimmedCount, brightness);

	if (firstPixel >= pixelCount - effectPixelCount)
	{
		step = -1;
	}
	else if (firstPixel <= 0)
	{
		step = 1;
	}

	firstPixel += step;
}

void Snake::HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount, uint8_t brightness)
{
	int halfCount = effectPixelCount / 2;
	int lastPixel = firstPixel + effectPixelCount - 1;
	int luminance;

	int luminanceStep = brightness / dimmedCount;
	for (int i = 0; i < halfCount; i++)
	{
		luminance = std::min(i * luminanceStep, (int)brightness);

		pixels->SetPixel(firstPixel + i, luminance, 0, 0, 0);
		pixels->SetPixel(lastPixel - i, luminance, 0, 0, 0);
	}

	WriteAndWait(pixels);
}