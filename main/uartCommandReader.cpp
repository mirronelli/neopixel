#include "uartCommandReader.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "cstring"

#define PIN_RTS  (UART_PIN_NO_CHANGE)
#define PIN_CTS  (UART_PIN_NO_CHANGE)

UartCommandReader::UartCommandReader(gpio_num_t rx, gpio_num_t tx)
{
	/* Configure parameters of an UART driver,
     * communication pins and install the driver */
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
	uart_param_config(UART_NUM_1, &uart_config);
	uart_set_pin(UART_NUM_1, tx, rx, PIN_RTS, PIN_CTS);
	uart_driver_install(UART_NUM_1, sizeof(buffer) * 2, 0, 0, NULL, 0);
}

UartCommandReader::~UartCommandReader(){}

void UartCommandReader::ReadCommand()
{
	xTaskCreate(UartCommandReader::PollForData, "Poll Uart", 1024, this, 10, NULL);
}

void UartCommandReader::PollForData(void* arg)
{
	UartCommandReader* instance = static_cast<UartCommandReader*>(arg);
	int totalLegth = 0;

	instance->signaled = false;
	while (!instance->signaled)
	{
		int inputLength = uart_read_bytes(UART_NUM_1, instance->buffer + totalLegth, sizeof(instance->buffer) - totalLegth, 20 / portTICK_RATE_MS);
       	uart_write_bytes(UART_NUM_1, ((const char *) instance->buffer) + totalLegth, inputLength);
		totalLegth += inputLength;

		if (instance->buffer[totalLegth - 1] == '\r')
		{
        	uart_write_bytes(UART_NUM_1, "\r\n", 2);

			instance->buffer[totalLegth-1] = 0;
			instance->signaled = true;
		}
		else if (totalLegth >= sizeof(instance->buffer))
		{
			totalLegth = 0;
			const char *errorMessage = "\r\noverflow\r\n" ;
			uart_write_bytes(UART_NUM_1, errorMessage, strlen(errorMessage));
		}
	}

	vTaskDelete(NULL);
}
