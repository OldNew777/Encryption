#include "SHA_3_256.h"

void SHA_3_256::GetHash(uint8_t data[], uint64_t len, uint8_t hash[HASH_LEN]) {
	uint64_t state[STATE_WORDS] = { 0 };

	for (size_t i = 0; i < len / BLOCK_SIZE; ++i)
		Iteration(data + i * BLOCK_SIZE, state);

	uint8_t buffer[BLOCK_SIZE] = { 0 };
	memcpy(buffer, data + len / BLOCK_SIZE * BLOCK_SIZE, len % BLOCK_SIZE);

	buffer[len % BLOCK_SIZE] |= 0x06;
	buffer[BLOCK_SIZE - 1] |= 0x80;

	Iteration(buffer, state);

	memcpy(hash, state, 32);
}
