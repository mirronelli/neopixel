//ESP libs
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_app_trace.h"
#include "string.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_sleep.h"

//my libs
#include "mWifiClient.h"
#include "mMqttClient.h"
#include "mTime.h"
#include "mI2c.h"

#include "main.h"
#include "neopixel.h"

//standard libs
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

static const int sleepTime = 150;
static const char *logTag = "Main";

static string mqttDataTopic = CONFIG_MQTT_DATA_TOPIC;
static string mqttWateringTopic = CONFIG_MQTT_WATERING_TOPIC;
static string mqttCommandTopic = CONFIG_MQTT_COMMAND_TOPIC;
static string mqttCommandReturnTopic; // will be the mqttCommandTopic + "Ret"
static const string mqttBrokerAddress = CONFIG_MQTT_BROKER_ADDRESS;
static const string timeZone = CONFIG_TIMEZONE;
static int wateringUpperBound = CONFIG_WATERING_UPPER_BOUND;
static int wateringDuration = CONFIG_WATERING_DURATION;
static int meteringInterval = CONFIG_METERING_INTERVAL;

static const gpio_num_t ledPin = GPIO_NUM_27;
static const gpio_num_t relayPin = GPIO_NUM_19;

// ads1115 i2c settings
static const gpio_num_t PIN_SDA = GPIO_NUM_21;
static const gpio_num_t PIN_CLK = GPIO_NUM_22;
static const uint32_t FREQUENCY = 100'000;
static const i2c_port_t PORT = I2C_NUM_0;
static const uint8_t ESP_SLAVE_ADDR = 0b0100'1000;
static const bool ACK_REQUIRED = true;

esp_timer_handle_t timer;

extern "C"
{
	void app_main(void);
}

void SnakePart(Pixels *pixels, int firstPixel, int effectPixelCount, int dimmedCount)
{
	int halfCount  = effectPixelCount / 2;
	int lastPixel = firstPixel + effectPixelCount - 1;
	int luminance;

	for (int i = 0; i < halfCount; i++)
	{
		if (i < dimmedCount)
			luminance = 1 << i;
		else
			luminance = 256;
			
		pixels->SetPixel(firstPixel + i, luminance - 1, 0, 0, 0);
		pixels->SetPixel(lastPixel - i, luminance - 1, 0, 0, 0);
	}
	
	pixels->Write();
}

void Snake(Pixels *pixels, int pixelCount, int effectPixelCount, int dimmedCount)
{
	for (int firstPixel = 0; firstPixel < pixelCount - effectPixelCount; firstPixel++)
	{
		SnakePart(pixels, firstPixel, effectPixelCount, dimmedCount);
	}

	for (int firstPixel = pixelCount - effectPixelCount; firstPixel >=0; firstPixel--)
	{
		SnakePart(pixels, firstPixel, effectPixelCount, dimmedCount);
	}
}

void app_main()
{
	//Main m = Main();
	//m.app_main();

	int pixelCount = 150;

	//NeoPixel neoPixel();
	Pixels* pixels = new Pixels(GPIO_NUM_15, pixelCount, RMT_CHANNEL_0);

	while(true)
	{
		Snake(pixels, pixelCount, 18, 8);
	}
}

void Main::app_main(void)
{
	//setup pins
	gpio_set_direction(relayPin, GPIO_MODE_OUTPUT);
	gpio_set_direction(ledPin, GPIO_MODE_OUTPUT);

	//setup i2c
	mI2cMaster master(PORT, PIN_SDA, PIN_CLK, ESP_SLAVE_ADDR, FREQUENCY);

	wifiClient = new mWifiClient(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD, 5);
	StartWifiOrSleep();

	mqttClient = new mMqttClient(mqttBrokerAddress);
	mqttCommandReturnTopic = mqttCommandTopic + "Ret";
	mqttClient->Subscribe(mqttCommandTopic, 1, Main::HandleMqttMessage, this);
	mTime::SetTimeZone(timeZone);

	esp_timer_create_args_t timerArgs = {
		.callback = Main::HandleTimer,
		.arg = this,
		.dispatch_method = ESP_TIMER_TASK,
		.name = "StopWatering",
	};
	esp_timer_create(&timerArgs, &timer);

	while (1)
	{
		// if for some reason wifi connectiom is lost, try to reconnect and wait up to 5 minutes
		if (!wifiClient->IsConnected())
			wifiClient->Connect(30'000);

		mDateTime dateTime = mTime::GetTime();
		string nowTime = mTime::GetTimeString(dateTime);

		Blink(200);
		Blink(200);
		Blink(200);

		//request measure, analog1_IN vs gnd, 4V max,
		master.WriteRegister16(1, 0b1'100'001'1'000'0'1'0'11);
		vTaskDelay(200 / portTICK_RATE_MS);
		int16_t soilHumidity = master.ReadRegister16(0) / 8;
		Blink(200);
		
		// WaterIfNeeded(nowTime, dateTime.Now, soilHumidity, wateringDuration, wateringUpperBound);

		string message = FormatDataMessage(nowTime, dateTime.Now, soilHumidity);
		mqttClient->Publish(mqttDataTopic, 0, 0, message);

		vTaskDelay(meteringInterval * 1000 / portTICK_RATE_MS);
	}
}

void Main::HandleTimer(void * instancePtr)
{
	Main *instance = (Main *)instancePtr;
	instance->StopWatering();
}

void Main::StopWatering()
{
	bool direction = false;
	gpio_set_level(relayPin, direction);

	mDateTime dateTime = mTime::GetTime();
	string nowTime = mTime::GetTimeString(dateTime);
	string message = FormatWateringMessage(nowTime, dateTime.Now, direction);
	mqttClient->Publish(mqttWateringTopic, 0, 0, message);
}

void Main::WaterIfNeeded(std::string timeString, time_t timeStamp, int value, int duration, int max)
{
	if (value >= max)
	{
		bool direction = true;
		gpio_set_level(relayPin, direction);

		string message = FormatWateringMessage(timeString, timeStamp, direction);
		mqttClient->Publish(mqttWateringTopic, 0, 0, message);
		esp_timer_start_once(timer, duration * 1'000'000);
	}
}

string Main::FormatWateringMessage(string timeString, time_t timeStamp, bool direction)
{
	ostringstream out;
	out << "{" << endl;
	out << "	\"Date\": \"" << timeString << "\"," << endl;
	out << "	\"Timestamp\": " << timeStamp << "," << endl;
	out << "	\"Message\": 	 "
		<< "\"Turning pump " << (direction ? "on" : "off") << ".\"" << endl;
	out << "}" << endl;
	return out.str();
}

string Main::FormatDataMessage(string timeString, time_t timeStamp, int value)
{
	ostringstream out;
	out << "{" << endl;
	out << "	\"time\": 		\"" << timeString << "\"," << endl;
	out << "	\"timestamp\": 	" << timeStamp << "," << endl;
	out << "	\"rawValue\": 	" << value << "," << endl;
	out << "	\"naturalValue\":0" ;
	out << "}" << endl;
	return out.str();
}

void Main::WakeUp()
{
	esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
	switch (reason)
	{
	case ESP_SLEEP_WAKEUP_UNDEFINED:
		ESP_LOGI(logTag, "Woken up because of nothing... What do you waaant from me!!!!");
		break;
	case ESP_SLEEP_WAKEUP_GPIO:
		ESP_LOGI(logTag, "Uaaaaahh, good mornig... Don't poke me!!");
		break;
	case ESP_SLEEP_WAKEUP_TIMER:
		ESP_LOGI(logTag, "Tick, tack, what time is it...");
		break;
	case ESP_SLEEP_WAKEUP_EXT0:
		ESP_LOGI(logTag, "Poke me once more to the zero..");
		break;
	default:
		ESP_LOGI(logTag, "Whaaat is happenniiiiing. %d", reason);
		break;
	}
}

void Main::Sleep()
{
	if (mqttClient != nullptr && mqttClient->IsConnected())
		mqttClient->Stop();

	if (wifiClient != nullptr && wifiClient->IsConnected())
		wifiClient->Disconnect();

	// printf("Enabling timer wakeup, %ds\n", sleepTime);
	// esp_sleep_enable_timer_wakeup(sleepTime * 1000000);
	// esp_sleep_enable_ext1_wakeup(1ULL << 15, ESP_EXT1_WAKEUP_ANY_HIGH);

	// esp_deep_sleep_start();
}

void Main::StartWifiOrSleep()
{
	int retryCount = 0;
	while (!wifiClient->Connect(++retryCount * 5000) && retryCount < 5)
	{
		ESP_LOGI(logTag, "Failed to connect. Retrying.. %d/5", retryCount);
	}

	if (!wifiClient->IsConnected())
	{
		ESP_LOGI(logTag, "Maximum connect retries reached. Cannot work without wifi. Sleeping.");
		Sleep();
	}
}

void Main::Blink(int delay)
{
	HalfBlink(delay);
	HalfBlink(delay);
}

void Main::HalfBlink(int delay)
{
	ledState = !ledState;
	gpio_set_level(ledPin, ledState);
	vTaskDelay(delay / (2 * portTICK_RATE_MS));
}

void Main::HandleMqttMessage(string topic, string message, void* arg)
{
	Main* instance = (Main*)arg;
	instance->ProcessCommand(message);
}

void Main::ProcessCommand(string command)
{
	size_t separatorPos = command.find(':');
	if (separatorPos != string::npos)
	{
		string variableName = command.substr(0, separatorPos);
		string valueString = command.substr(separatorPos + 1, string::npos);
		try
		{
			if (variableName == "duration")
			{
				wateringDuration = std::stoi(valueString);
			}
			else if (variableName == "upperBound")
			{
				wateringUpperBound = std::stoi(valueString);
			}
			else if (variableName == "interval")
			{
				meteringInterval = std::stoi(valueString);
			}
			else if (variableName == "dataTopic")
			{
				mqttDataTopic = valueString;
			}
			else if (variableName == "wateringTopic")
			{
				mqttWateringTopic = valueString;
			}

			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "Variable %s set to value %s.", variableName.c_str(), valueString.c_str());
		}
		catch (std::exception &e)
		{
			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "Could not parse value %s for variable %s.", valueString.c_str(), variableName.c_str());
		}
	}
	else 
	{
		if (command == "getConfig")
		{
			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "%s:\t%d", "duration", wateringDuration);
			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "%s:\t%d", "upperBound", wateringUpperBound);
			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "%s:\t%d", "interval", meteringInterval);
			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "%s:\t%s", "dataTopic", mqttDataTopic.c_str());
			mqttClient->PublishFormat(mqttCommandReturnTopic, 0, 0, "%s:\t%s", "wateringTopic", mqttWateringTopic.c_str());
		}
	}
}
