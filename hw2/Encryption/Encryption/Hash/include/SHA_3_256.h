#pragma once

#include <stdint.h>

class SHA_3_256
{
	uint32_t GetHash(uint8_t* src, uint32_t lenSrc, uint8_t* dst);
};