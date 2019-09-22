#include "police.h"

Police::Police(int pixelCount, int refreshSpeed):
	Effect(pixelCount, refreshSpeed),
	halfPixels(pixelCount / 2){};

void Police::Run(Pixels *pixels)
{
	if (firstRun)
	{
		FirstRun(pixels);
	} 
	else
	{
		int lastPixel = firstPixel + halfPixels;
		Pixel pixel = pixels->GetPixel(firstPixel);

		if (pixel.red == 0)
		{
			pixels->SetPixel(firstPixel, 255, 0, 0, 0);
			pixels->SetPixel(lastPixel, 0, 0, 255, 0);
		}
		else 
		{
			pixels->SetPixel(lastPixel, 255, 0, 0, 0);
			pixels->SetPixel(firstPixel, 0, 0, 255, 0);
		}

		firstPixel++;
		if (firstPixel >= halfPixels)
		{
			firstPixel = 0;
		}
	}
	WriteAndWait(pixels);
}

void Police::FirstRun(Pixels *pixels)
{
	for (int i = 0; i < pixelCount; i++)
	{
		uint8_t red, blue;

		if (i < halfPixels)
		{
			red = 255; blue = 0;
		} else {
			blue = 255; red = 0;
		}

		pixels->SetPixel(i, red, 0, blue, 0);
	}

	firstRun = false;
}