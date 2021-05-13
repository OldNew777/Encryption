#pragma once

#include <stdint.h>
#include <stdio.h>

//#define likely(x) __builtin_expect(!!(x), 1)
//#define unlikely(x) __builtin_expect(!!(x), 0)

class MathFunction
{
public:
	template<class T>
	inline static T RightRotate(T x, const size_t i) {
		return x >> i | x << (sizeof(T) * 8 - i);
	}
	template<class T>
	inline static T LeftRotate(T x, const size_t i) {
		return x << i | x >> (sizeof(T) * 8 - i);
	}

	inline static void PrintHex(FILE* _Stream, uint8_t* data, uint32_t len) {
		if (_Stream == stdout) {
			for (int i = 0; i < len; ++i)
				printf("%02x", int(data[i]) & 0xff);
			printf("\n");
		}
		else {
			for (int i = 0; i < len; ++i)
				fprintf(_Stream, "%02x", int(data[i]) & 0xff);
			fprintf(_Stream, "\n");
		}
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
			if (b & 1)
				ans = AddGF(ans, a);
			carry = a & 0x80;
			a <<= 1;
			if (carry)
				a = SubGF(a, 0x1b);
			b >>= 1;
		}
		return ans;
	}
};