#include "DataGenerator.h"

#include <random>
#include <time.h>

static bool randomInit = false;

uint8_t* DataGenerator(uint32_t len)
{
	uint8_t* data = new uint8_t[len];
	if (!randomInit) {
		randomInit = true;
		srand(time(0));
	}

	for (int i = 0; i < len; ++i)
		data[i] = rand() & 0xff;

	return data;
}

uint8_t* VisiableStringGenerator(uint32_t len)
{
	uint8_t* data = new uint8_t[len];
	if (!randomInit) {
		randomInit = true;
		srand(time(0));
	}

	for (int i = 0; i < len; ++i)
		data[i] = (rand() % 95) + 32;

	return data;
}
