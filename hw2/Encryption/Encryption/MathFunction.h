#pragma once

#include <stdint.h>
#include <stdio.h>
#include <algorithm>

extern const uint8_t MutGfPreset_0x01[256];
extern const uint8_t MutGfPreset_0x02[256];
extern const uint8_t MutGfPreset_0x03[256];
extern const uint8_t MutGfPreset_0x09[256];
extern const uint8_t MutGfPreset_0x0b[256];
extern const uint8_t MutGfPreset_0x0d[256];
extern const uint8_t MutGfPreset_0x0e[256];

class MathFunction
{
private:
	static uint8_t* MutGfPresetPtr[15];

public:
	template<class T>
	inline static T RightRotate(T x, const size_t i) {
		return x >> i | x << (sizeof(T) * 8 - i);
	}
	template<class T>
	inline static T LeftRotate(T x, const size_t i) {
		return x << i | x >> (sizeof(T) * 8 - i);
	}

	inline static void PrintString(FILE* _Stream, uint8_t* data, uint32_t len, bool Backslash = false) {
		char* str = (char*)data;
		for (int i = 0; i < len; ++i) {
			if (Backslash && (str[i] == '\\' || str[i] == '\"' || str[i] == '\''))
				fprintf(_Stream, "%c", '\\');
			fprintf(_Stream, "%c", str[i]);
		}
	}
	inline static void PrintHex(FILE* _Stream, uint8_t* data, uint32_t len, bool format = false) {
		for (int i = 0; i < len; ++i) {
			fprintf(_Stream, "%02x", int(data[i]) & 0xff);
			if (format && (i & 3) == 3)
				fprintf(_Stream, " ");
			if (format && (i & 15) == 15)
				fprintf(_Stream, "\n");
		}
		if (format && (len & 15))
			fprintf(_Stream, "\n");
	}

	inline static uint8_t AddGF(uint8_t a, uint8_t b) {
		return a ^ b;
	}
	inline static uint8_t SubGF(uint8_t a, uint8_t b) {
		return a ^ b;
	}
	inline static uint8_t MutGF(uint8_t a, uint8_t b) {
		uint8_t ans = 0, carry = 0;

		for (int i = 0; i < 8; ++i) {
			if (a & 1) {
				//ans ^= b;
				ans = AddGF(ans, b);
			}
			carry = b & 0x80;
			b <<= 1;
			if (carry) {
				//b ^= 0x1b;
				b = SubGF(b, 0x1b);
			}
			a >>= 1;
		}

		return ans;
	}

	inline static uint8_t MutAddGF(uint8_t A[4], uint8_t state[4]) {
		return
			MutGfPresetPtr[A[0]][state[0]] ^
			MutGfPresetPtr[A[1]][state[1]] ^
			MutGfPresetPtr[A[2]][state[2]] ^
			MutGfPresetPtr[A[3]][state[3]];
	}
	static void MutGfPresetInit() {
		int A[7] = { 0x01, 0x02, 0x03, 0x09, 0x0b, 0x0d, 0x0e };
		for (int i = 0; i < 7; ++i) {
			int num = A[i];
			printf("uint8_t MutGfPreset_0x%02x[256] = {", num);
			for (int j = 0; j < 256; ++j) {
				if ((j & 15) == 0)
					printf("\n");
				printf("0x%02x, ", int(MutGF(num, j)));
			}
			printf("\n};\n\n");
		}
	}
};