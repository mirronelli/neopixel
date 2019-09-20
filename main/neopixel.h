#ifndef mNeoPixel_h
#define mNeoPixel_h

#include "driver/gpio.h"
#include "driver/rmt.h"

struct Pixel 
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
};

class Pixels
{
	private:
		rmt_item32_t* rmtItems;
		uint8_t* pixelData;
		rmt_channel_t channel;
		int bitCount;

		int zeroBitHighTime = 0;
		int zeroBitLowTime = 0;
		int oneBitHighTime = 0;
		int oneBitLowTime = 0;

		void SetupPixel(int index);
		void SetupPixelBit(int index);

	public:
		enum class StripType {
			ws2812,
			ws6812
		};

		int PixelCount;
		Pixels(gpio_num_t pin, int pixelCount, StripType stripType, rmt_channel_t channel);
		void SetPixel(int index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
		void SetPixel(int index, Pixel pixel);
		Pixel GetPixel(int index);
		void Write();
};
#endif