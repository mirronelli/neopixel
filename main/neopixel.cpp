#include "neopixel.h"
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"

NeoPixel::NeoPixel(gpio_num_t pin, int pixelCount, rmt_channel_t channel)
{
	this->pixelCount = pixelCount;
	this->rmtItems = new rmt_item32_t[pixelCount*32];
	this->channel = channel;

	rmt_config_t config;
	config.rmt_mode = RMT_MODE_TX;
	config.channel = channel;
	config.gpio_num = pin;
	config.mem_block_num = 3;
	config.tx_config.loop_en = false;
	config.tx_config.carrier_en = false;
	config.tx_config.idle_output_en = true;
	config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
	config.clk_div = 2;

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
}

void NeoPixel::Write(PixelData* pixels)
{
	int pixelIndex = 0;
	for (PixelData* pixel = pixels; pixel < pixels + this->pixelCount; pixel++)
	{
		uint32_t pixelData = (*pixel).Data;
		uint32_t mask = 1 << 31;
		
		//ESP_LOGI("1", "pixel: %#010x", pixelData);

		for (char bit = 0; bit < 32; bit++)
		{
			//ESP_LOGI("4", "Processing bit:%d", pixelIndex*32 + bit);
			rmt_item32_t *item = &rmtItems[pixelIndex*32 + bit];

			if ((pixelData & mask) > 0)
			{
			//	ESP_LOGI("3", "Bit %d is on", bit);
				item->duration0 = 52;
				item->level0 = 1;
				item->duration1 = 52;
				item->level1 = 0;
			}
			else
			{
			//	ESP_LOGI("3", "Bit %d is off", bit);
				item->duration0 = 14;
				item->level0 = 1;
				item->duration1 = 52;
				item->level1 = 0;
			}
			
			mask >>= 1;
		}

		pixelIndex++;
	}

	ESP_LOGI("2", "channel %d, pixelCount: %d", channel, this->pixelCount*32);
	ESP_ERROR_CHECK(rmt_write_items(this->channel, rmtItems, this->pixelCount*32, true));
}

PixelData::PixelData(){
	this->Data = 0;
}

PixelData::PixelData(char red, char green, char blue, char white)
{
	this->Data = 0;
	this->Data |= red << 16;
	this->Data |= green << 24;
	this->Data |= blue << 8;
	this->Data |= white;
}

void PixelData::Color(char value, char shift)
{
	uint32_t shifted =  value << shift;
	uint32_t clearMask = ~(255 << shift);
	this->Data &= clearMask;
	this->Data |= shifted;
}

void PixelData::Red(char value)
{
	this->Color(value, 16);
}

void PixelData::Green(char value)
{
	this->Color(value, 24);
}

void PixelData::Blue(char value)
{
	this->Color(value, 8);
}

void PixelData::White(char value)
{
	this->Color(value, 0);
}

Pixels::Pixels(gpio_num_t pin, int count, rmt_channel_t channel)
{
	this->PixelCount = count;
	this->bitCount = count * 32;
	this->rmtItems = new rmt_item32_t[bitCount];
	this->channel = channel;

	rmt_config_t config;
	config.rmt_mode = RMT_MODE_TX;
	config.channel = channel;
	config.gpio_num = pin;
	config.mem_block_num = 3;
	config.tx_config.loop_en = false;
	config.tx_config.carrier_en = false;
	config.tx_config.idle_output_en = true;
	config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
	config.clk_div = 2;

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

	ESP_LOGI("1","1");
	for (rmt_item32_t* item = rmtItems; item < rmtItems + bitCount; item++)
	{
		item->duration0 = 14;
		item->level0 = 1;
		item->duration1 = 52;
		item->level1 = 0;
	}
	ESP_LOGI("1","2");
	this->Write();
	ESP_LOGI("1","3");
}

void Pixels::Write()
{
	ESP_ERROR_CHECK(rmt_write_items(this->channel, this->rmtItems, this->bitCount, true));
}

void Pixels::SetPixel(int index, char red, char green, char blue, char white)
{
	uint32_t pixelData = (green << 24) | (red << 16) | (blue << 8) | white;
	uint32_t mask = 1 << 31;  
	for (int i = index * 32; i < (index * 32) + 32; i++)
	{
		this->rmtItems[i].duration0 =  (pixelData & mask) ? 52 : 14;
		mask >>= 1;
	}
}
