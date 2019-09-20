#include "neopixel.h"
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"
#include "malloc.h"
#include "freertos/task.h"

Pixels::Pixels(gpio_num_t pin, int count, StripType stripType, rmt_channel_t channel)
{
	this->PixelCount = count;
	this->bitCount = count * 32;
	this->rmtItems = new rmt_item32_t[bitCount];
	this->pixelData = new uint8_t[count * 4];
	this->channel = channel;

	// ws6812 and ws2812 timings
	//https://cdn.solarbotics.com/products/datasheets/sk6812rgbw.pdf
	//https://learn.adafruit.com/adafruit-neopixel-uberguide/advanced-coding
	//	
	//    false:24 | true:48   
	//   false:72 | true:48
	if (stripType == StripType::ws6812)
	{
		oneBitHighTime = 48;
		oneBitLowTime = 48;
		zeroBitHighTime = 24;
		zeroBitLowTime = 72;
	}

	rmt_config_t config;
	config.rmt_mode = RMT_MODE_TX;
	config.channel = channel;
	config.gpio_num = pin;
	config.mem_block_num = 3;
	config.tx_config.loop_en = false;
	config.tx_config.carrier_en = false;
	config.tx_config.idle_output_en = true;
	config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
	config.clk_div = 1;

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

	for (int i = 0; i < PixelCount; i++)
	{
		this->SetupPixel(i);
	}	
	
	this->Write();
}

void Pixels::Write()
{
	rmt_write_items(this->channel, this->rmtItems, this->bitCount, true);
}

void Pixels::SetupPixel(int index)
{
	int firstByteIndex = index * 4;

	this->pixelData[firstByteIndex + 0] = 0;
	this->pixelData[firstByteIndex + 1] = 0;
	this->pixelData[firstByteIndex + 2] = 0;
	this->pixelData[firstByteIndex + 3] = 0;
	for (int i = index * 4 * 8; i < (index + 1) * 4 * 8; i++)
	{
		SetupPixelBit(i);
	}
}

void Pixels::SetupPixelBit(int index)
{
	this->rmtItems[index].level0 = 1;
	this->rmtItems[index].level1 = 0;
	this->rmtItems[index].duration0 = zeroBitHighTime;
	this->rmtItems[index].duration1 = zeroBitLowTime;
}

Pixel Pixels::GetPixel(int index)
{
	Pixel pixel;
	int firstByteIndex = index * 4;

	pixel.white 	= this->pixelData[firstByteIndex + 0];
	pixel.blue 		= this->pixelData[firstByteIndex + 1];
	pixel.red 		= this->pixelData[firstByteIndex + 2];
	pixel.green 	= this->pixelData[firstByteIndex + 3];
	return pixel;
}

void Pixels::SetPixel(int index, Pixel pixel)
{
	SetPixel(index, pixel.red, pixel.green, pixel.blue, pixel.white);
}

void Pixels::SetPixel(int index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
	int firstByteIndex = index * 4;

	this->pixelData[firstByteIndex + 0] = white;
	this->pixelData[firstByteIndex + 1] = blue;
	this->pixelData[firstByteIndex + 2] = red;
	this->pixelData[firstByteIndex + 3] = green;

	int startBit = index * 32;
	//uint32_t widePixelData = (green << 24) | (red << 16) | (blue << 8) | white;
	uint32_t* widePixelData = (uint32_t*)(pixelData + firstByteIndex);
	uint32_t mask = 1 << 31;  

	for (int i = startBit; i < startBit + 32; i++)
	{
		if ((*widePixelData)&mask)
		{
			this->rmtItems[i].duration0 = oneBitHighTime;
			this->rmtItems[i].duration1 = oneBitLowTime;
		}
		else
		{
			this->rmtItems[i].duration0 = zeroBitHighTime;
			this->rmtItems[i].duration1 = zeroBitLowTime;
		}
		mask >>= 1;
	}
}