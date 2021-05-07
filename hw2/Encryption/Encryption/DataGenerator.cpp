#include "DataGenerator.h"

#include <omp.h>
#include <random>
#include <time.h>

uint8_t* DataGenerator(uint32_t len)
{
	uint8_t* data = new uint8_t[len];
	static bool randomInit = false;
	if (!randomInit) {
		randomInit = true;
		srand(time(0));
	}

#pragma omp parallel for
	for (int i = 0; i < len; ++i)
		data[i] = rand() & 0xff;

	return data;
}
