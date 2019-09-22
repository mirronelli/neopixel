#include "neopixel.h"
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"
#include "malloc.h"
#include "freertos/task.h"
#include <stdexcept>
#include "math.h"

Pixels::Pixels(gpio_num_t pin, int pixelCount, StripType stripType, rmt_channel_t channel, double gamma) : pin(pin),
																										   pixelCount(pixelCount),
																										   bitCount(pixelCount * 32),
																										   channel(channel),
																										   gamma(gamma),
																										   stripType(stripType),
																										   rmtItems(new rmt_item32_t[bitCount]),
																										   pixelData(new uint8_t[pixelCount * 4])

{
	SetupTiming();
	SetupRmt();
	SetupPixels();
	SetupGammaTable();
	Write();
}

void Pixels::Write()
{
	rmt_write_items(this->channel, this->rmtItems, this->bitCount, true);
}

void Pixels::Clear()
{
	SetupPixels();
}

void Pixels::SetupTiming()
{
	// ws6812 and ws2812 timings
	//https://cdn.solarbotics.com/products/datasheets/sk6812rgbw.pdf
	//https://learn.adafruit.com/adafruit-neopixel-uberguide/advanced-coding
	
	//		  FALSE	   TRUE	
	// HIGH    24   | true:48
	// LOW     72   | true:48

	// other strip types not yet supported
	if (stripType == StripType::ws6812)
	{
		oneBitHighTime = 48;
		oneBitLowTime = 48;
		zeroBitHighTime = 24;
		zeroBitLowTime = 72;
	}
	else
	{
		throw std::invalid_argument("strip type not supported");
	}
	
}

void Pixels::SetupGammaTable()
{
	for (int i = 0; i <= 255; i++)
	{
		gammaTable[i] = pow((float)i / 255, gamma) * 255;
	}
}

void Pixels::SetupPixels()
{
	for (int i = 0; i < pixelCount; i++)
	{
		this->SetupPixel(i);
	}	
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

void Pixels::SetupRmt()
{
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
}

Pixel Pixels::GetPixel(int index)
{
	if (index > pixelCount || index < 0)
	{
		throw std::out_of_range("index out of range");
	}

	int firstByteIndex = index * 4;

	Pixel pixel;
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
	if (index > pixelCount || index < 0)
	{
		throw std::out_of_range("index out of range");
	}
	
	int firstByteIndex = index * 4;
	this->pixelData[firstByteIndex + 0] = white;
	this->pixelData[firstByteIndex + 1] = blue;
	this->pixelData[firstByteIndex + 2] = red;
	this->pixelData[firstByteIndex + 3] = green;

	uint32_t widePixelData = gammaTable[green] << 24 | gammaTable[red] << 16 | gammaTable[blue] << 8 | gammaTable[white];

	uint32_t mask = 1 << 31;  
	int startBit = index * 32;
	for (int i = startBit; i < startBit + 32; i++)
	{
		if (widePixelData & mask)
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