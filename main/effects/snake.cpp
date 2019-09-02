#include "snake.h"

void Snake::HalfSnake(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount)
{
	int halfCount  = effectPixelCount / 2;
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
	
	pixels->Write();
}

void Snake::Run(Pixels *pixels, int pixelCount, int effectPixelCount, int dimmedCount)
{
	for (int firstPixel = 0; firstPixel < pixelCount - effectPixelCount; firstPixel++)
	{
		HalfSnake(pixels, firstPixel, effectPixelCount, dimmedCount);
	}

	for (int firstPixel = pixelCount - effectPixelCount; firstPixel >=0; firstPixel--)
	{
		HalfSnake(pixels, firstPixel, effectPixelCount, dimmedCount);
	}
}