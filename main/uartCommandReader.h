#ifndef UartCommandReader_h
#define UartCommandReader_h

#include "esp_types.h"
#include "driver/gpio.h"

class UartCommandReader
{
public:
	UartCommandReader(gpio_num_t rx, gpio_num_t tx);
	~UartCommandReader();
	void ReadCommand();
	bool signaled;
	uint8_t buffer[128];
	size_t commandLength;

private:
	static void PollForData(void *arg);
};

#endif