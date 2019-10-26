#include "snake.h"
#include <algorithm>

Snake::Snake(int pixelCount, int refreshSpeed, int effectPixelCount, int dimmedCount, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) : Effect(pixelCount, refreshSpeed),
																											effectPixelCount(effectPixelCount),
																											dimmedCount(dimmedCount),
																											red(red),
																											green(green),
																											blue(blue),
																											white(white){};

void Snake::Run(Pixels* pixels)
{
	HalfSnake(pixels, firstPixel);

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

void Snake::HalfSnake(Pixels *pixels, int firstPixel)
{
	int halfCount = effectPixelCount / 2;
	int lastPixel = firstPixel + effectPixelCount - 1;
	int luminance;

	int luminanceStep = 255 / dimmedCount;
	for (int i = 0; i < halfCount; i++)
	{
		luminance = std::min(i * luminanceStep, 255);

		pixels->SetPixel(firstPixel + i, (luminance * red) / 255, (luminance * green) / 255, (luminance * blue) / 255, (luminance * white) / 255);
		pixels->SetPixel(lastPixel  - i, (luminance * red) / 255, (luminance * green) / 255, (luminance * blue) / 255, (luminance * white) / 255);
	}

	WriteAndWait(pixels);
}