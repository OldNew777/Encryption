#include "SHA_3_256.h"

void SHA_3_256::absorb(uint8_t data[], uint64_t n) {
	for (uint64_t i = 0; i < n; ++i) {
		state.bytes[absorbed++] ^= data[i];

		if (absorbed == RATE) {
			keccak();
			absorbed = 0;
		}
	}

	padpoint = absorbed;
}

void SHA_3_256::squeeze(uint8_t digest[DIGEST]) {
	state.bytes[padpoint] ^= 0x06;
	state.bytes[RATE - 1] ^= 0x80;

	keccak();

	for (int i = 0; i < DIGEST; ++i) {
		digest[i] = state.bytes[i];
	}
}

void SHA_3_256::GetHash(uint8_t data[], uint64_t n, uint8_t digest[DIGEST]) {
	memset(state.bytes, 0, WIDTH);
	padpoint = absorbed = 0;
	absorb(data, n);
	squeeze(digest);
}
