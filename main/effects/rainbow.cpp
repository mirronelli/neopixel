#include "rainbow.h"
#include "math.h"

Rainbow::Rainbow(int pixelCount, int refreshSpeed, uint8_t brightness) : Effect(pixelCount, refreshSpeed),
																		 phaseLength(pixelCount / 6),
																		 phaseStepCoefficient(pow(brightness, 1.0 / phaseLength)),
																		 brightness(brightness){};

void Rainbow::Run(Pixels *pixels)
{
	if (firstRun)
	{
		FirstRun(pixels);
	}
	else
	{
		Pixel lastPixel = pixels->GetPixel(pixelCount - 1);
		for (int i = pixelCount - 1; i > 0; i--)
		{
			Pixel movedPixel = pixels->GetPixel(i - 1);
			pixels->SetPixel(i, movedPixel);
		}
		pixels->SetPixel(0, lastPixel);
	}
	
	WriteAndWait(pixels);
}

void Rainbow::FirstRun(Pixels *pixels)
{
	uint8_t r,g,b;

	for (int i = 0; i < pixelCount; i++)
	{
		int phaseIndex = i / phaseLength;
		int phaseStep = i % phaseLength;
		double phaseStepSize = (brightness * 1.0) / phaseLength;
		uint8_t ascendingValue = phaseStep * phaseStepSize;
		uint8_t descendingValue = (phaseLength - phaseStep) * phaseStepSize;

		switch (phaseIndex)
		{
		case 0:
			r = brightness;
			g = ascendingValue;
			b = 0;
			break;
		case 1:
			r = descendingValue;
			g = brightness;
			b = 0;
			break;
		case 2:
			r = 0;
			g = brightness;
			b = ascendingValue;
			break;
		case 3:
			r = 0;
			g = descendingValue;
			b = brightness;
			break;
		case 4:
			r = ascendingValue;
			g = 0;
			b = brightness;
			break;
		case 5:
			r = brightness;
			g = 0;
			b = descendingValue;
			break;
		default:
			r = 0;
			g = 0;
			b = 0;
			break;
		}

		pixels->SetPixel(i, r, g, b, 0);
	}

	firstRun = false;
}