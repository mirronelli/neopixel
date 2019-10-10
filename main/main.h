#include "effects/effect.h"
#include <string>

class Main
{
public:
	void Run();

private:
	void ProcessCommand(std::string command);
	void SetEffect(std::string effect);
	static void HandleMqttMessage(std::string topic, std::string message, void* arg);

	Effect* currentEffect;
	Pixels* pixels;
};