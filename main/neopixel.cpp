#include "neopixel.h"
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"

Pixels::Pixels(gpio_num_t pin, int count, StripType stripType, rmt_channel_t channel)
{
	this->PixelCount = count;
	this->bitCount = count * 32;
	this->rmtItems = new rmt_item32_t[bitCount];
	this->channel = channel;

	// ws6812 and ws2812 timings
	//https://cdn.solarbotics.com/products/datasheets/sk6812rgbw.pdf
	//https://learn.adafruit.com/adafruit-neopixel-uberguide/advanced-coding
	//	
	//    false:24 | true:48   
	//   false:72 | true:48
	if (stripType == StripType::ws6812)
	{
		oneBitHightTime = 40;
		oneBitLowTime = 40;
		zeroBitHighTime = 15;
		zeroBitLowTime = 65;
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

	for (int i = 0; i < PixelCount*32; i++)
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
	this->rmtItems[index].level0 = 1;
	this->rmtItems[index].level1 = 0;
	this->rmtItems[index].duration0 = zeroBitHighTime;
	this->rmtItems[index].duration1 = zeroBitLowTime;
}

void Pixels::SetPixel(int index, char red, char green, char blue, char white)
{
	int startBit = index * 32;
	uint32_t pixelData = (green << 24) | (red << 16) | (blue << 8) | white;
	uint32_t mask = 1 << 31;  

	for (int i = startBit; i < startBit + 32; i++)
	{
		if (pixelData & mask)
		{
			this->rmtItems[i].duration0 = oneBitHightTime;
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