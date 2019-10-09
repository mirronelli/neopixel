//ESP libs
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string>

//my libs
#include "main.h"
#include "neopixel.h"
#include "effects/snake.h"
#include "effects/stars.h"
#include "effects/police.h"
#include "effects/rainbow.h"
#include "effects/effectFactory.h"
#include "uartCommandReader.h"

using namespace std;

extern "C"
{
	void app_main(void);
}

void app_main()
{
	Main main = Main();
	main.Run();
}

void Main::Run()
{
	UartCommandReader commandReader = UartCommandReader((gpio_num_t)CONFIG_COMMAND_RX_GPIO, (gpio_num_t)CONFIG_COMMAND_TX_GPIO);
	commandReader.ReadCommand();

	pixels = new Pixels((gpio_num_t)CONFIG_LED_PIN, CONFIG_LED_COUNT, Pixels::StripType::ws6812, RMT_CHANNEL_0, 2.8);
	currentEffect = EffectFactory::CreateEffect(CONFIG_DEFAULT_EFFECT_COMMAND, CONFIG_LED_COUNT, CONFIG_FRAME_DELAY);

	while (true)
	{
		currentEffect->Run(pixels);
		if (commandReader.signaled)
		{
			string command((char*)commandReader.buffer);
			ProcessCommand(command);
			commandReader.ReadCommand();
		}
	}
}

void Main::ProcessCommand(string command)
{
	printf("got command: %s\n", command.c_str());
	size_t separatorPos = command.find(':');
	if (separatorPos != string::npos)
	{
		string commandName = command.substr(0, separatorPos);
		string parameterString = command.substr(separatorPos + 1, string::npos);

		if (commandName == "effect")
		{
			SetEffect(parameterString);
		}
	}
}

void Main::SetEffect(string effect)
{
	printf("setting effect: \"%s\"\n", effect.c_str());

	try
	{
		Effect *newEffect = EffectFactory::CreateEffect(effect, pixelCount, delay);
		pixels->Clear();
		delete (currentEffect);
		currentEffect = newEffect;
	}
	catch (std::exception& e)
	{
		printf("could not set effect, giving up command\n");
	}
}