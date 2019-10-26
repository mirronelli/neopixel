#include "solid.h"

Solid::Solid(int pixelCount, int refreshSpeed, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) : 
	Effect(pixelCount, refreshSpeed),
	red(red),
	green(green),
	blue(blue),
	white(white)
{}

void Solid::Run(Pixels* pixels)
{
	for (int i = 0; i < pixelCount; i++)
	{
		Pixel pixel = pixels->GetPixel(i);
		pixel.red = red;
		pixel.green = green;
		pixel.blue = blue;
		pixel.white = white;
		pixels->SetPixel(i, pixel);
	}
	
	WriteAndWait(pixels);
}