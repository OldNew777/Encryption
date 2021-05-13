#pragma once

#include <stdint.h>
#include <string.h>

#include "MathFunction.h"

#define ROUNDS  24    // Number of KECCAK rounds to perform for SHA3-256.
#define WIDTH   200   // 1600-bit width in bytes.
#define LANES   25    // The state is an unrolled 5x5 array of 64-bit lanes.
#define RATE    136   // 1600-bit width - 512-bit capacity in bytes.
#define DIGEST  32    // 256-bit digest in bytes.

union SHA_State {
	uint64_t words[LANES];
	uint8_t bytes[WIDTH];
};

class SHA_3_256
{
private:
	int padpoint = 0;
	int absorbed = 0;
	SHA_State state;

public:
	inline void theta() {
		uint64_t C[5] = { 0 };
		uint64_t D[5] = { 0 };

		for (int i = 0; i < 5; ++i) {
			C[i] = state.words[i] ^
				state.words[i + 5] ^
				state.words[i + 10] ^
				state.words[i + 15] ^
				state.words[i + 20];
		}

		for (int i = 0; i < 5; ++i)
			D[i] = C[(i + 4) % 5] ^ MathFunction::LeftRotate(C[(i + 1) % 5], 1);

		for (int i = 0; i < 25; ++i)
			state.words[i] ^= D[i % 5];
	}

	inline void rho() {
		static const int rotations[25] = {
			 0,  1, 62, 28, 27,
			36, 44,  6, 55, 20,
			 3, 10, 43, 25, 39,
			41, 45, 15, 21,  8,
			18,  2, 61, 56, 14
		};

		for (int i = 0; i < 25; ++i)
			state.words[i] = MathFunction::LeftRotate(state.words[i], rotations[i]);
	}

	inline void pi() {
		static constexpr int switcheroo[25] = {
			 0, 10, 20,  5, 15,
			16,  1, 11, 21,  6,
			 7, 17,  2, 12, 22,
			23,  8, 18,  3, 13,
			14, 24,  9, 19,  4
		};

		uint64_t temp[25] = { 0 };

		memcpy(temp, state.words, 25 * sizeof(uint64_t));

		for (int i = 0; i < 25; ++i)
			state.words[switcheroo[i]] = temp[i];
	}

	inline void chi() {
		uint64_t temp[5] = { 0 };

		for (int j = 0; j < 25; j += 5) {
			memcpy(temp, &state.words[j], 5 * sizeof(uint64_t));

			for (int i = 0; i < 5; ++i)
				state.words[i + j] ^= (~temp[(i + 1) % 5]) & temp[(i + 2) % 5];
		}
	}

	inline void iota(uint8_t round) {
		static constexpr uint64_t constants[24] = {
			0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
			0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
			0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
			0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
			0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
			0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
			0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
			0x8000000000008080, 0x0000000080000001, 0x8000000080008008
		};

		state.words[0] ^= constants[round];
	}

	inline void keccak() {
		for (int i = 0; i < ROUNDS; ++i) {
			theta();
			rho();
			pi();
			chi();
			iota(i);
		}
	}

	void absorb(uint8_t data[], uint64_t n);
	void squeeze(uint8_t digest[DIGEST]);
	void GetHash(uint8_t data[], uint64_t n, uint8_t digest[DIGEST]);
};