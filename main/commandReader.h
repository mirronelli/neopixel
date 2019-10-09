#ifndef CommandReader_h
#define CommandReader_h

#include "esp_types.h"

class CommandReader
{
public:
	CommandReader();
	~CommandReader();
	void ReadCommand();
	bool signaled;
	uint8_t buffer[128];
	size_t commandLength;

private:
	static void PollForData(void *arg);
};

#endif