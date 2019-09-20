#include "stars.h"
#include "math.h"

void Stars::Run()
{
	double redFadeCoefficient = pow(red, 1.0 / fadeSpeed);
	double greenFadeCoefficient = pow(green, 1.0 / fadeSpeed);
	double blueFadeCoefficient = pow(blue, 1.0 / fadeSpeed);
	double whiteFadeCoefficient = pow(white, 1.0 / fadeSpeed);

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
			if (redFadeCoefficient > 0)
			{
				pixel.red /= redFadeCoefficient;
			}

			if (greenFadeCoefficient > 1)
			{
				pixel.green /= greenFadeCoefficient;
			}

			if (blueFadeCoefficient > 1)
			{
				pixel.blue /= blueFadeCoefficient;
			}

			if (whiteFadeCoefficient > 1)
			{
				pixel.white /= whiteFadeCoefficient;
			}
		}

		pixels->SetPixel(i, pixel);
	}
	
	WriteAndWait();
}