#include "AES_128.h"
#include "DataGenerator.h"

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

void AES_128::TestCipher(uint8_t key[16], uint8_t* src, uint8_t* dst)
{
	AES_128 aes;
	aes.ExpandKey(key);
	aes.Cipher(src, dst);
	aes.Decipher(dst, src);
}

uint8_t* AES_128::Encrypt(uint8_t* src, uint32_t lenSrc, uint32_t& lenDst, uint8_t key[16])
{
	ExpandKey(key);

	lenDst = (lenSrc & ~15) + 32;

	uint8_t* dst = new uint8_t[lenDst], * XOR = DataGenerator(16), padding = 16 - (lenSrc & 15);
	memcpy(dst, XOR, 16);
	delete[] XOR;
	memcpy(dst + 16, src, lenSrc);
	memset(dst + lenSrc + 16, padding, lenDst - lenSrc - 16);

	for (int i = 16; i < lenDst; i += 16) {
		for (int j = 0; j < 16; ++j)
			dst[i + j] ^= dst[i - 16 + j];
		Cipher(dst + i, dst + i);
	}

	printf("Encrypt::padding = %d\n", int(padding));

	return dst;
}

uint8_t* AES_128::Decrypt(uint8_t* src, uint32_t lenSrc, uint32_t& lenDst, uint8_t key[16])
{
	assert(lenSrc & 15 == 0 && lenSrc >= 32);

	ExpandKey(key);

	uint8_t* data = new uint8_t[lenSrc];
	memcpy(data, src, lenSrc);

#pragma omp parallel for
	for (int i = 16; i < lenSrc; i += 16)
		Decipher(data + i, data + i);

#pragma omp parallel for
	for (int i = 16; i < lenSrc; i += 16) {
		for (int j = 0; j < 16; ++j)
			data[i + j] ^= src[i - 16 + j];
	}

	for (int i = 0; i < 16; ++i)
		printf("Decrypt::padding = %d\n", int(data[lenSrc - 1 - i]));

	lenDst = lenSrc - data[lenSrc - 1] - 16;
	uint8_t* dst = new uint8_t[lenDst];
	memcpy(dst, data + 16, lenDst);

	delete[] data;
	return dst;
}
