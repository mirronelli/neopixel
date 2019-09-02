#include "mMqttClient.h"
#include "mWifiClient.h"

class Main
{
public:
	void app_main();

private:
	void StartWifi();
	void ProcessCommand(std::string command);
	static void HandleMqttMessage(std::string topic, std::string message, void* arg);

	mMqttClient *mqttClient = nullptr;
	mWifiClient *wifiClient = nullptr;
};