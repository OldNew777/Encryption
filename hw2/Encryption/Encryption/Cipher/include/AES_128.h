#pragma once

#include <stdint.h>
#include <string.h>

#include "MathFunction.h"

union AES_Expansive_Key {
	uint64_t lwords[22];
	uint32_t words[44];
	uint8_t bytes[176];
};


extern const uint8_t SBox[256];
extern const uint8_t SBoxInv[256];


class AES_128
{
private:
	AES_Expansive_Key expansiveKey;

	inline void BytesReplace(uint8_t state[4][4]) {
		state[0][0] = SBox[state[0][0]];
		state[0][1] = SBox[state[0][1]];
		state[0][2] = SBox[state[0][2]];
		state[0][3] = SBox[state[0][3]];
		state[1][0] = SBox[state[1][0]];
		state[1][1] = SBox[state[1][1]];
		state[1][2] = SBox[state[1][2]];
		state[1][3] = SBox[state[1][3]];
		state[2][0] = SBox[state[2][0]];
		state[2][1] = SBox[state[2][1]];
		state[2][2] = SBox[state[2][2]];
		state[2][3] = SBox[state[2][3]];
		state[3][0] = SBox[state[3][0]];
		state[3][1] = SBox[state[3][1]];
		state[3][2] = SBox[state[3][2]];
		state[3][3] = SBox[state[3][3]];
	}
	inline void BytesReplaceInv(uint8_t state[4][4]) {
		state[0][0] = SBoxInv[state[0][0]];
		state[0][1] = SBoxInv[state[0][1]];
		state[0][2] = SBoxInv[state[0][2]];
		state[0][3] = SBoxInv[state[0][3]];
		state[1][0] = SBoxInv[state[1][0]];
		state[1][1] = SBoxInv[state[1][1]];
		state[1][2] = SBoxInv[state[1][2]];
		state[1][3] = SBoxInv[state[1][3]];
		state[2][0] = SBoxInv[state[2][0]];
		state[2][1] = SBoxInv[state[2][1]];
		state[2][2] = SBoxInv[state[2][2]];
		state[2][3] = SBoxInv[state[2][3]];
		state[3][0] = SBoxInv[state[3][0]];
		state[3][1] = SBoxInv[state[3][1]];
		state[3][2] = SBoxInv[state[3][2]];
		state[3][3] = SBoxInv[state[3][3]];
	}

	/// <summary>
	/// Actually ColumnShift, because we have reversed column and row
	/// </summary>
	inline void RowsShift(uint8_t state[4][4]) {
		uint8_t temp = state[0][1];
		state[0][1] = state[1][1],
			state[1][1] = state[2][1],
			state[2][1] = state[3][1],
			state[3][1] = temp;

		std::swap(state[0][2], state[2][2]);
		std::swap(state[1][2], state[3][2]);

		temp = state[0][3],
			state[0][3] = state[3][3],
			state[3][3] = state[2][3],
			state[2][3] = state[1][3],
			state[1][3] = temp;
	}
	/// <summary>
	/// Actually ColumnShiftInv, because we have reversed column and row
	/// </summary>
	inline void RowsShiftInv(uint8_t state[4][4]) {
		uint8_t temp = state[0][1];
		state[0][1] = state[3][1],
			state[3][1] = state[2][1],
			state[2][1] = state[1][1],
			state[1][1] = temp;

		std::swap(state[0][2], state[2][2]);
		std::swap(state[1][2], state[3][2]);

		temp = state[0][3],
			state[0][3] = state[1][3],
			state[1][3] = state[2][3],
			state[2][3] = state[3][3],
			state[3][3] = temp;
	}

	/// <summary>
	/// Actually RowsMix, because we have reversed column and row
	/// </summary>
	inline void ColumnsMix(uint8_t state[4][4]) {
		static constexpr uint8_t A[7] = { 0x03, 0x01, 0x01, 0x02, 0x03, 0x01, 0x01 };
		uint8_t temp[4];

		//for (int i = 0; i < 4; ++i) {
		//	memcpy(temp, state[i], 4);

		//	for (int j = 0; j < 4; ++j)
		//		state[i][j] = MathFunction::MutAddGF((uint8_t*)A + 3 - j, temp);
		//}

		memcpy(temp, state[0], 4);
		state[0][0] = MutGfPreset_0x02[temp[0]] ^ MutGfPreset_0x03[temp[1]] ^ temp[2] ^ temp[3];
		state[0][1] = temp[0] ^ MutGfPreset_0x02[temp[1]] ^ MutGfPreset_0x03[temp[2]] ^ temp[3];
		state[0][2] = temp[0] ^ temp[1] ^ MutGfPreset_0x02[temp[2]] ^ MutGfPreset_0x03[temp[3]];
		state[0][3] = MutGfPreset_0x03[temp[0]] ^ temp[1] ^ temp[2] ^ MutGfPreset_0x02[temp[3]];
		memcpy(temp, state[1], 4);
		state[1][0] = MutGfPreset_0x02[temp[0]] ^ MutGfPreset_0x03[temp[1]] ^ temp[2] ^ temp[3];
		state[1][1] = temp[0] ^ MutGfPreset_0x02[temp[1]] ^ MutGfPreset_0x03[temp[2]] ^ temp[3];
		state[1][2] = temp[0] ^ temp[1] ^ MutGfPreset_0x02[temp[2]] ^ MutGfPreset_0x03[temp[3]];
		state[1][3] = MutGfPreset_0x03[temp[0]] ^ temp[1] ^ temp[2] ^ MutGfPreset_0x02[temp[3]];
		memcpy(temp, state[2], 4);
		state[2][0] = MutGfPreset_0x02[temp[0]] ^ MutGfPreset_0x03[temp[1]] ^ temp[2] ^ temp[3];
		state[2][1] = temp[0] ^ MutGfPreset_0x02[temp[1]] ^ MutGfPreset_0x03[temp[2]] ^ temp[3];
		state[2][2] = temp[0] ^ temp[1] ^ MutGfPreset_0x02[temp[2]] ^ MutGfPreset_0x03[temp[3]];
		state[2][3] = MutGfPreset_0x03[temp[0]] ^ temp[1] ^ temp[2] ^ MutGfPreset_0x02[temp[3]];
		memcpy(temp, state[3], 4);
		state[3][0] = MutGfPreset_0x02[temp[0]] ^ MutGfPreset_0x03[temp[1]] ^ temp[2] ^ temp[3];
		state[3][1] = temp[0] ^ MutGfPreset_0x02[temp[1]] ^ MutGfPreset_0x03[temp[2]] ^ temp[3];
		state[3][2] = temp[0] ^ temp[1] ^ MutGfPreset_0x02[temp[2]] ^ MutGfPreset_0x03[temp[3]];
		state[3][3] = MutGfPreset_0x03[temp[0]] ^ temp[1] ^ temp[2] ^ MutGfPreset_0x02[temp[3]];
	}
	/// <summary>
	/// Actually RowsMixInv, because we have reversed column and row
	/// </summary>
	inline void ColumnsMixInv(uint8_t state[4][4]) {
		static constexpr uint8_t AInv[7] = { 0x0b, 0x0d, 0x09, 0x0e, 0x0b, 0x0d, 0x09 };
		uint8_t temp[4];

		//for (int i = 0; i < 4; ++i) {
		//	memcpy(temp, state[i], 4);

		//	for (int j = 0; j < 4; ++j)
		//		state[i][j] = MathFunction::MutAddGF((uint8_t*)AInv + 3 - j, temp);
		//}

		memcpy(temp, state[0], 4);
		state[0][0] = MutGfPreset_0x0e[temp[0]] ^ MutGfPreset_0x0b[temp[1]] ^ MutGfPreset_0x0d[temp[2]] ^ MutGfPreset_0x09[temp[3]];
		state[0][1] = MutGfPreset_0x09[temp[0]] ^ MutGfPreset_0x0e[temp[1]] ^ MutGfPreset_0x0b[temp[2]] ^ MutGfPreset_0x0d[temp[3]];
		state[0][2] = MutGfPreset_0x0d[temp[0]] ^ MutGfPreset_0x09[temp[1]] ^ MutGfPreset_0x0e[temp[2]] ^ MutGfPreset_0x0b[temp[3]];
		state[0][3] = MutGfPreset_0x0b[temp[0]] ^ MutGfPreset_0x0d[temp[1]] ^ MutGfPreset_0x09[temp[2]] ^ MutGfPreset_0x0e[temp[3]];
		memcpy(temp, state[1], 4);
		state[1][0] = MutGfPreset_0x0e[temp[0]] ^ MutGfPreset_0x0b[temp[1]] ^ MutGfPreset_0x0d[temp[2]] ^ MutGfPreset_0x09[temp[3]];
		state[1][1] = MutGfPreset_0x09[temp[0]] ^ MutGfPreset_0x0e[temp[1]] ^ MutGfPreset_0x0b[temp[2]] ^ MutGfPreset_0x0d[temp[3]];
		state[1][2] = MutGfPreset_0x0d[temp[0]] ^ MutGfPreset_0x09[temp[1]] ^ MutGfPreset_0x0e[temp[2]] ^ MutGfPreset_0x0b[temp[3]];
		state[1][3] = MutGfPreset_0x0b[temp[0]] ^ MutGfPreset_0x0d[temp[1]] ^ MutGfPreset_0x09[temp[2]] ^ MutGfPreset_0x0e[temp[3]];
		memcpy(temp, state[2], 4);
		state[2][0] = MutGfPreset_0x0e[temp[0]] ^ MutGfPreset_0x0b[temp[1]] ^ MutGfPreset_0x0d[temp[2]] ^ MutGfPreset_0x09[temp[3]];
		state[2][1] = MutGfPreset_0x09[temp[0]] ^ MutGfPreset_0x0e[temp[1]] ^ MutGfPreset_0x0b[temp[2]] ^ MutGfPreset_0x0d[temp[3]];
		state[2][2] = MutGfPreset_0x0d[temp[0]] ^ MutGfPreset_0x09[temp[1]] ^ MutGfPreset_0x0e[temp[2]] ^ MutGfPreset_0x0b[temp[3]];
		state[2][3] = MutGfPreset_0x0b[temp[0]] ^ MutGfPreset_0x0d[temp[1]] ^ MutGfPreset_0x09[temp[2]] ^ MutGfPreset_0x0e[temp[3]];
		memcpy(temp, state[3], 4);
		state[3][0] = MutGfPreset_0x0e[temp[0]] ^ MutGfPreset_0x0b[temp[1]] ^ MutGfPreset_0x0d[temp[2]] ^ MutGfPreset_0x09[temp[3]];
		state[3][1] = MutGfPreset_0x09[temp[0]] ^ MutGfPreset_0x0e[temp[1]] ^ MutGfPreset_0x0b[temp[2]] ^ MutGfPreset_0x0d[temp[3]];
		state[3][2] = MutGfPreset_0x0d[temp[0]] ^ MutGfPreset_0x09[temp[1]] ^ MutGfPreset_0x0e[temp[2]] ^ MutGfPreset_0x0b[temp[3]];
		state[3][3] = MutGfPreset_0x0b[temp[0]] ^ MutGfPreset_0x0d[temp[1]] ^ MutGfPreset_0x09[temp[2]] ^ MutGfPreset_0x0e[temp[3]];
	}

	inline void AddRoundKey(uint8_t state[4][4], int round) {
		uint64_t
			& state1 = *(uint64_t*)state,
			& state2 = *((uint64_t*)state + 1);

		state1 ^= expansiveKey.lwords[round << 1];
		state2 ^= expansiveKey.lwords[(round << 1) + 1];

		//int baseIndex = round << 4;
		//for (int i = 0; i < 4; ++i) {
		//	int base = baseIndex + (i << 2);
		//	for (int j = 0; j < 4; ++j)
		//		state[i][j] ^= expansiveKey.bytes[base + j];
		//}
	}

	inline uint32_t T(uint32_t w, int round) {
		static constexpr uint8_t Rcon[11] = {
			0x00, 0x01, 0x02, 0x04, 0x08,
			0x10, 0x20, 0x40, 0x80, 0x1B,
			0x36,
		};

		uint8_t* data = (uint8_t*)&w;

		// 字节左移 + S盒
		uint8_t tmp = SBox[data[0]];
		for (int i = 0; i < 3; ++i)
			data[i] = SBox[data[i + 1]];
		data[3] = tmp;

		// 轮常量异或
		data[0] ^= Rcon[round];

		return w;
	}

	void ExpandKey(uint8_t key[16]);

	inline void Cipher(uint8_t* src, uint8_t* dst) {
		uint8_t state[4][4];
		memcpy(state, src, 16);

		AddRoundKey(state, 0);

		for (int round = 1; round < 10; ++round) {
			BytesReplace(state);
			RowsShift(state);
			ColumnsMix(state);
			AddRoundKey(state, round);
		}

		BytesReplace(state);
		RowsShift(state);
		AddRoundKey(state, 10);

		memcpy(dst, state, 16);
	}
	inline void Decipher(uint8_t* src, uint8_t* dst) {
		uint8_t state[4][4];
		memcpy(state, src, 16);

		AddRoundKey(state, 10);

		for (int round = 9; round > 0; --round) {
			RowsShiftInv(state);
			BytesReplaceInv(state);
			AddRoundKey(state, round);
			ColumnsMixInv(state);
		}

		RowsShiftInv(state);
		BytesReplaceInv(state);
		AddRoundKey(state, 0);

		memcpy(dst, state, 16);
	}

	AES_128& operator=(const AES_128& src);

public:
	static uint8_t* Encrypt(uint8_t* src, uint32_t lenSrc, uint32_t& lenDst, uint8_t key[16], uint8_t IV[16]);
	static uint8_t* Decrypt(uint8_t* src, uint32_t lenSrc, uint32_t& lenDst, uint8_t key[16], uint8_t IV[16]);

	static void CipherBlock(uint8_t key[16], uint8_t* src, uint8_t* dst);

	static bool PrintFlag;
};
