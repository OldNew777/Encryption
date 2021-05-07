#pragma once

#include <stdint.h>

class AES_128
{
	uint32_t Encrypt(uint8_t* src, uint32_t lenSrc, uint8_t* dst);
	uint32_t Decrypt(uint8_t* src, uint32_t lenSrc, uint8_t* dst);
};

