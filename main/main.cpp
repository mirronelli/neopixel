//ESP libs
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string>

//my libs
// #include "mWifiClient.h"
// #include "mMqttClient.h"
#include "mBle.h"

#include "main.h"
#include "neopixel.h"
#include "effects/snake.h"
#include "effects/stars.h"
#include "effects/police.h"
#include "effects/rainbow.h"
#include "effects/effectFactory.h"
#include "nvs_flash.h"

using namespace std;

//static const char *logTag = "Main";

static string mqttCommandTopic = CONFIG_MQTT_COMMAND_TOPIC;
static string mqttCommandReturnTopic; // will be the mqttCommandTopic + "Ret"
static const string mqttBrokerAddress = CONFIG_MQTT_BROKER_ADDRESS;

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
	// wifiClient = new mWifiClient(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD, 5);
	// StartWifi();
	// mqttClient = new mMqttClient(mqttBrokerAddress);
	// mqttCommandReturnTopic = mqttCommandTopic + "Ret";
	// mqttClient->Subscribe(mqttCommandTopic, 1, Main::HandleMqttMessage, this);
	nvs_flash_init();
	mBle_init();

	pixelCount = 150;
	delay = 50;
	pixels = new Pixels(GPIO_NUM_13, pixelCount, Pixels::StripType::ws6812, RMT_CHANNEL_0, 2.8);
	currentEffect = EffectFactory::CreateEffect("snake", 150, 5);

	while (true)
	{
		currentEffect->Run(pixels);
	}
}

void Main::StartWifi()
{
	// int retryCount = 0;
	// while (!wifiClient->Connect(++retryCount * 5000) && retryCount < 5)
	// {
	// 	ESP_LOGI(logTag, "Failed to connect. Retrying.. %d/5", retryCount);
	// }
}

void Main::HandleMqttMessage(string topic, string message, void* arg)
{
	// Main* instance = (Main*)arg;
	// instance->ProcessCommand(message);
}

void Main::ProcessCommand(string command)
{
	// printf("got command: %s\n", command.c_str());
	// size_t separatorPos = command.find(':');
	// if (separatorPos != string::npos)
	// {
	// 	string commandName = command.substr(0, separatorPos);
	// 	string parameterString = command.substr(separatorPos + 1, string::npos);

	// 	if (commandName == "effect")
	// 	{
	// 		SetEffect(parameterString);
	// 	}
	// }
}

void Main::SetEffect(string effect)
{
	// printf("setting effect: \"%s\"\n", effect.c_str());

	// try
	// {
	// 	Effect *newEffect = EffectFactory::CreateEffect(effect, pixelCount, delay);
	// 	pixels->Clear();
	// 	delete (currentEffect);
	// 	currentEffect = newEffect;
	// }
	// catch (std::exception& e)
	// {
	// 	printf("could not set effect, giving up command\n");
	// }
}