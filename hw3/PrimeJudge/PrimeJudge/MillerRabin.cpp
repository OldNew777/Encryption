#include "MillerRabin.h"

bool MillerRabin::JudgePrime(const uint512_t& n)
{
	// n - 1 = m * 2^s
	if (n <= 1)
		return false;
	uint512_t m(n), n_1 = n - 1;
	--m;
	int s = 0;
	while (!(m & 1)) {
		++s;
		m >>= 1;
	}
	for (int primeIndex = 0; primeIndex < 50; ++primeIndex) {
		if (n == Prime[primeIndex] || n_1 <= Prime[primeIndex])
			return true;
		uint512_t y = PowMod(Prime[primeIndex], m, n);
		if (y != 1 && y != n_1)
			return false;
		for (int i = 0; i < s; ++i) {
			uint512_t y_2 = (y * y) % n;
			if (y_2 == 1 && y != 1 && y != n_1)
				return false;
			y = y_2;
		}
	}
	return true;
}

uint32_t MillerRabin::Prime[50] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
	31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
	73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
	127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
	179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
};