#include "RC4.h"

#include <omp.h>
#include <assert.h>
#include <algorithm>

void RC4::Init(uint8_t* key, uint32_t keyLen)
{
	assert(key != nullptr);
	assert(keyLen <= 256);

	uint8_t k[256];
#pragma omp parallel for
	for (int i = 0; i < 256; ++i) {
		s[i] = i;
		k[i] = key[i % keyLen];
	}
	uint32_t j = 0;
	for (uint32_t i = 0; i < 256; ++i) {
		j = (j + s[i] + k[i]) & 0xff;
		std::swap(s[i], s[j]);
	}

	this->encrypt_index_i = 0;
	this->encrypt_index_j = 0;
}

void RC4::Encrypt(uint8_t* data, uint32_t lenSrc)
{
	uint32_t i = this->encrypt_index_i, j = this->encrypt_index_j;
	for (uint32_t k = 0; k < lenSrc; ++k) {
		i = (i + 1) & 0xff;
		j = (j + s[i]) & 0xff;
		std::swap(s[i], s[j]);
		data[k] ^= (s[i] + s[j]) & 0xff;
	}
	this->encrypt_index_i = i;
	this->encrypt_index_j = j;
}

void RC4::PrintSBox() const
{
	printf("\n****************************************************************\n");
	for (uint32_t i = 0; i < 16; ++i) {
		for (uint32_t j = 0; j < 16; ++j)
			printf("% 4d", uint32_t(s[i * 16 + j]));
		printf("\n");
	}
	printf("****************************************************************\n\n");
}
