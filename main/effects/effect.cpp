#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "effect.h"
#include "../neopixel.h"

void Effect::WriteAndWait()
{
	pixels->Write();
	
	if (refreshSpeed > 0)
	{
		vTaskDelay(refreshSpeed / portTICK_PERIOD_MS);
	}
}