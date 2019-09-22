#include "snake.h"
#include <algorithm>

void Snake::Run()
{
	HalfSnake(pixels, firstPixel, effectPixelCount, dimmedCount);

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

void Snake::HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount)
{
	int halfCount = effectPixelCount / 2;
	int lastPixel = firstPixel + effectPixelCount - 1;
	int luminance;

	int luminanceStep = 256 / dimmedCount;
	for (int i = 0; i < halfCount; i++)
	{
		luminance = std::min(i * luminanceStep, 255);

		pixels->SetPixel(firstPixel + i, luminance, 0, 0, 0);
		pixels->SetPixel(lastPixel - i, luminance, 0, 0, 0);
	}

	WriteAndWait();
}