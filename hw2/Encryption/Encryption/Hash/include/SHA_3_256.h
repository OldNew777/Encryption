#pragma once

#include <stdint.h>
#include <string.h>

#include "MathFunction.h"

#define ROUNDS        24
#define STATE_BYTES   200
#define STATE_WORDS   25
#define BLOCK_SIZE    136
#define HASH_LEN      32


class SHA_3_256
{
private:
	inline void Iteration(const uint8_t* data, uint64_t state[]) {
		for (size_t i = 0; i <= 16; ++i) {
			state[i] ^=
				uint64_t(data[(i << 3) + 0]) << 0 |
				uint64_t(data[(i << 3) + 1]) << 8 |
				uint64_t(data[(i << 3) + 2]) << 16 |
				uint64_t(data[(i << 3) + 3]) << 24 |
				uint64_t(data[(i << 3) + 4]) << 32 |
				uint64_t(data[(i << 3) + 5]) << 40 |
				uint64_t(data[(i << 3) + 6]) << 48 |
				uint64_t(data[(i << 3) + 7]) << 56;
		}
		Permutation(state);
	}

	inline void Permutation(uint64_t state[]) {
		static constexpr uint64_t Rcon[24] = {
			0x0000000000000001, 0x0000000000008082, 0x800000000000808A,
			0x8000000080008000, 0x000000000000808B, 0x0000000080000001,
			0x8000000080008081, 0x8000000000008009, 0x000000000000008A,
			0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
			0x000000008000808B, 0x800000000000008B, 0x8000000000008089,
			0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
			0x000000000000800A, 0x800000008000000A, 0x8000000080008081,
			0x8000000000008080, 0x0000000080000001, 0x8000000080008008,
		};
		for (size_t round = 0; round < 24; round++) {
			KeccakTheta(state);

			state[1] = MathFunction::LeftRotate(state[1], 1);
			state[2] = MathFunction::LeftRotate(state[2], 62);
			state[3] = MathFunction::LeftRotate(state[3], 28);
			state[4] = MathFunction::LeftRotate(state[4], 27);
			state[5] = MathFunction::LeftRotate(state[5], 36);
			state[6] = MathFunction::LeftRotate(state[6], 44);
			state[7] = MathFunction::LeftRotate(state[7], 6);
			state[8] = MathFunction::LeftRotate(state[8], 55);
			state[9] = MathFunction::LeftRotate(state[9], 20);
			state[10] = MathFunction::LeftRotate(state[10], 3);
			state[11] = MathFunction::LeftRotate(state[11], 10);
			state[12] = MathFunction::LeftRotate(state[12], 43);
			state[13] = MathFunction::LeftRotate(state[13], 25);
			state[14] = MathFunction::LeftRotate(state[14], 39);
			state[15] = MathFunction::LeftRotate(state[15], 41);
			state[16] = MathFunction::LeftRotate(state[16], 45);
			state[17] = MathFunction::LeftRotate(state[17], 15);
			state[18] = MathFunction::LeftRotate(state[18], 21);
			state[19] = MathFunction::LeftRotate(state[19], 8);
			state[20] = MathFunction::LeftRotate(state[20], 18);
			state[21] = MathFunction::LeftRotate(state[21], 2);
			state[22] = MathFunction::LeftRotate(state[22], 61);
			state[23] = MathFunction::LeftRotate(state[23], 56);
			state[24] = MathFunction::LeftRotate(state[24], 14);

			KeccakPi(state);
			KeccakChi(state);

			state[0] ^= Rcon[round];
		}
	}

	inline void KeccakChi(uint64_t A[]) {
		for (size_t i = 0; i < 25; i += 5) {
			uint64_t A0 = A[i], A1 = A[i + 1];
			A[i] ^= ~A1 & A[i + 2];
			A[i + 1] ^= ~A[i + 2] & A[i + 3];
			A[i + 2] ^= ~A[i + 3] & A[i + 4];
			A[i + 3] ^= ~A[i + 4] & A0;
			A[i + 4] ^= ~A0 & A1;
		}
	}
	inline void KeccakPi(uint64_t A[]) {
		uint64_t A1 = A[1];
		A[1] = A[6], A[6] = A[9], A[9] = A[22], A[22] = A[14],
			A[14] = A[20], A[20] = A[2], A[2] = A[12], A[12] = A[13],
			A[13] = A[19], A[19] = A[23], A[23] = A[15], A[15] = A[4],
			A[4] = A[24], A[24] = A[21], A[21] = A[8], A[8] = A[16],
			A[16] = A[5], A[5] = A[3], A[3] = A[18], A[18] = A[17],
			A[17] = A[11], A[11] = A[7], A[7] = A[10], A[10] = A1;
	}
	inline void KeccakTheta(uint64_t A[]) {
		uint64_t C[5], D[5];
		for (size_t i = 0; i < 5; ++i)
			C[i] = A[i] ^ A[i + 5] ^ A[i + 10] ^ A[i + 15] ^ A[i + 20];

		D[0] = MathFunction::LeftRotate(C[1], 1) ^ C[4];
		D[1] = MathFunction::LeftRotate(C[2], 1) ^ C[0];
		D[2] = MathFunction::LeftRotate(C[3], 1) ^ C[1];
		D[3] = MathFunction::LeftRotate(C[4], 1) ^ C[2];
		D[4] = MathFunction::LeftRotate(C[0], 1) ^ C[3];

		for (size_t i = 0; i < 5; ++i) {
			A[i] ^= D[i];
			A[i + 5] ^= D[i];
			A[i + 10] ^= D[i];
			A[i + 15] ^= D[i];
			A[i + 20] ^= D[i];
		}
	}

public:

	void GetHash(uint8_t data[], uint64_t len, uint8_t hash[HASH_LEN]);
};