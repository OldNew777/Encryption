#pragma once

#include <stdint.h>

class RC4
{
private:
	uint8_t s[256];
	uint32_t encrypt_index_i, encrypt_index_j;

public:
	void Init(uint8_t* key, uint32_t keyLen);
	void Encrypt(uint8_t* data, uint32_t lenSrc);

	void PrintSBox() const;
};