#ifndef mNeoPixel_h
#define mNeoPixel_h

#include "driver/gpio.h"
#include "driver/rmt.h"

class Pixels
{
	private:
		rmt_item32_t* rmtItems;
		rmt_channel_t channel;
		int bitCount;

	public:
		int PixelCount;
		Pixels(gpio_num_t pin, int pixelCount, rmt_channel_t channel);
		void SetPixel(int index, char red, char green, char blue, char white);
		void Write();
};

class PixelData
{
private:
	void Color(char value, char shift);

public:
	uint32_t Data;
	PixelData();
	PixelData(char red, char green, char blue, char white);
	void Red(char value);
	void Green(char value);
	void Blue(char value);
	void White(char value);
};


class NeoPixel 
{
public:

	NeoPixel(gpio_num_t pin, int pixelCount, rmt_channel_t channel);
	void Write(PixelData *pixels);

private:
	int pixelCount;
	rmt_item32_t* rmtItems;
	rmt_channel_t channel;
};

#endif