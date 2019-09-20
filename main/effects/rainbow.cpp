#include "rainbow.h"
#include "math.h"

void Rainbow::Run()
{
	if (firstRun)
	{
		FirstRun();
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
	
	WriteAndWait();
}

void Rainbow::FirstRun()
{
	uint8_t r,g,b;

	for (int i = 0; i < pixelCount; i++)
	{
		int phaseIndex = i / phaseLength;
		int phaseStep = i % phaseLength;
		uint8_t ascendingValue = pow(phaseStepCoefficient, phaseStep);
		uint8_t descendingValue = pow(phaseStepCoefficient, phaseLength - phaseStep);

		switch (phaseIndex)
		{
		case 0:
			r = 255;
			g = ascendingValue;
			b = 0;
			break;
		case 1:
			r = descendingValue;
			g = 255;
			b = 0;
			break;
		case 2:
			r = 0;
			g = 255;
			b = ascendingValue;
			break;
		case 3:
			r = 0;
			g = descendingValue;
			b = 255;
			break;
		case 4:
			r = ascendingValue;
			g = 0;
			b = 255;
			break;
		case 5:
			r = 255;
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