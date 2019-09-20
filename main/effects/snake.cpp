#include "snake.h"

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

	for (int i = 0; i < halfCount; i++)
	{
		if (i < dimmedCount)
			luminance = 1 << i;
		else
			luminance = 256;

		pixels->SetPixel(firstPixel + i, luminance - 1, 0, 0, 0);
		pixels->SetPixel(lastPixel - i, luminance - 1, 0, 0, 0);
	}

	WriteAndWait();
}