#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "police.h"
#include "../neopixel.h"

void Police::Run()
{
	int lastPixel = firstPixel + halfPixels;

	for (int i=0; i < pixelCount; i++)
	{
		int red, blue;

		if ((i >= firstPixel && i < lastPixel) || (i < firstPixel - halfPixels))
		{
			red = 255; blue = 0;
		} else {
			blue = 255; red = 0;
		}

		pixels->SetPixel(i, red, 0, blue, 0);
	}

	firstPixel ++;
	if (firstPixel > pixelCount)
	{
		firstPixel = 0;
	}

	WriteAndWait();
}