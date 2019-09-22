#include "mMqttClient.h"
#include "mWifiClient.h"
#include "effects/effect.h"
#include <string>

class Main
{
public:
	void Run();

private:
	void StartWifi();
	void ProcessCommand(std::string command);
	void SetEffect(std::string effect);
	static void HandleMqttMessage(std::string topic, std::string message, void* arg);

	mMqttClient *mqttClient = nullptr;
	mWifiClient *wifiClient = nullptr;

	Effect* currentEffect;
	Pixels* pixels;
	int pixelCount;
	int delay;
};