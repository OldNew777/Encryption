#include "AES_128.h"

#include <omp.h>
#include <assert.h>

void AES_128::ExpandKey(uint8_t key[16]) {
	memcpy(expansiveKey.bytes, key, 16);

	for (int i = 4; i < 44; ++i) {
		if (i & 0b11)
			expansiveKey.words[i] = expansiveKey.words[i - 4] ^ expansiveKey.words[i - 1];
		else
			expansiveKey.words[i] = expansiveKey.words[i - 4] ^ T(expansiveKey.words[i - 1], i >> 2);
	}
}

uint8_t* AES_128::Encrypt(uint8_t* src, uint32_t lenSrc, uint32_t& lenDst, uint8_t key[16], uint8_t IV[16])
{
	ExpandKey(key);

	uint32_t len = (lenSrc & ~15) + 16;
	lenDst = len;
	uint8_t* dst = new uint8_t[len], * XOR = IV, padding = 16 - (lenSrc & 15);
	memcpy(dst, src, lenSrc);
	memset(dst + lenSrc, padding, len - lenSrc);

	for (int i = 0; i < len; i += 16) {
		for (int j = 0; j < 16; ++j)
			dst[i + j] ^= XOR[j];
		Cipher(dst + i, dst + i);
		XOR = dst + i;
	}

	return dst;
}

uint8_t* AES_128::Decrypt(uint8_t* src, uint32_t lenSrc, uint32_t& lenDst, uint8_t key[16], uint8_t IV[16])
{
	assert(lenSrc & 15 == 0 && lenSrc > 16);

	ExpandKey(key);

	uint8_t* data = new uint8_t[lenSrc];
	memcpy(data, src, lenSrc);

#pragma omp parallel for
	for (int i = 0; i < lenSrc; i += 16)
		Decipher(data + i, data + i);

	for (int j = 0; j < 16; ++j)
		data[j] ^= IV[j];
	for (int i = 16; i < lenSrc; i += 16) {
		for (int j = 0; j < 16; ++j)
			data[i + j] ^= src[i - 16 + j];
	}

	lenDst = lenSrc - data[lenSrc - 1];
	uint8_t* dst = new uint8_t[lenDst];
	memcpy(dst, data, lenDst);

	delete[] data;
	return dst;
}
