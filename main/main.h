#include "mMqttClient.h"
#include "mWifiClient.h"

class Main
{
public:
	void app_main();

private:
	void StartWifiOrSleep();
	void WakeUp();
	void Sleep();
	void Blink(int delay);
	void HalfBlink(int delay);

	void WaterIfNeeded(std::string timeString, time_t timeStamp, int humidity, int duration, int max);
	void StopWatering();
	void ProcessCommand(std::string command);

	static void HandleTimer(void * instancePtr);
	static void HandleMqttMessage(std::string topic, std::string message, void* arg);

	std::string FormatDataMessage(std::string timeString, time_t timeStamp, int value);
	std::string FormatWateringMessage(std::string timeString, time_t timeStamp, bool direction);

	mMqttClient *mqttClient = nullptr;
	mWifiClient *wifiClient = nullptr;
	bool ledState = 0;
};