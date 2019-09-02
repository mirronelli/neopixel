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

		int zeroBitHighTime = 0;
		int zeroBitLowTime = 0;
		int oneBitHightTime = 0;
		int oneBitLowTime = 0;

	public:
		enum class StripType {
			ws2812,
			ws6812
		};

		int PixelCount;
		Pixels(gpio_num_t pin, int pixelCount, StripType stripType, rmt_channel_t channel);
		void SetPixel(int index, char red, char green, char blue, char white);
		void SetupPixel(int index);
		void Write();
};
#endif