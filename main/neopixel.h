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
	public:
		enum class StripType {
			ws2812,
			ws6812
		};

		Pixels(gpio_num_t pin, int pixelCount, StripType stripType, rmt_channel_t channel, double gamma);

		void SetPixel(int index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
		void SetPixel(int index, Pixel pixel);
		Pixel GetPixel(int index);

		void Write();
		void Clear();

	private:
		gpio_num_t pin;
		int pixelCount;
		int bitCount;
		rmt_channel_t channel;
		double gamma;
		StripType stripType;
		rmt_item32_t* rmtItems;
		uint8_t* pixelData;

		int zeroBitHighTime = 0;
		int zeroBitLowTime = 0;
		int oneBitHighTime = 0;
		int oneBitLowTime = 0;

		void SetupPixels();
		void SetupPixel(int index);
		void SetupPixelBit(int index);
		void SetupGammaTable();
		void SetupRmt();
		void SetupTiming();

		uint8_t gammaTable[255];

};
#endif