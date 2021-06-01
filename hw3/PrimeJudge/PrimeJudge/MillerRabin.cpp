#include "MillerRabin.h"

bool MillerRabin::JudgePrime(const uint512_t& n)
{
	// n - 1 = m * 2^s
	if (n == 2)
		return true;
	if (n <= 1 || !n.Test(0))
		return false;
	uint1024_t n_1 = n - 1, m(n_1), n_1024 = n;
	int s = 0;
	while (!(m & 1)) {
		++s;
		m >>= 1;
	}
	for (int primeIndex = 0; primeIndex < 50; ++primeIndex) {
		uint32_t prime = Prime[primeIndex];
		if (n == prime || n_1 <= prime)
			return true;
		uint1024_t y = PowMod(prime, m, n_1024);
		for (int i = 0; i < s; ++i) {
			uint1024_t y_2 = (y * y) % n_1024;
			if (y_2 == 1 && y != 1 && y != n_1)
				return false;
			y = y_2;
		}
		if (y != 1)
			return false;
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


uint1024_t MultiplyMod(const uint1024_t& a, const uint1024_t& b, const uint1024_t& mod)
{
	uint1024_t temp(b);
	uint1024_t ans(0), res(a);
	while (temp != 0) {
		if (temp.Test(0))
			ans = (ans + res) % mod;
		res <<= 1;
		temp >>= 1;
	}
	return ans;
}

uint1024_t PowMod(const uint1024_t& a, const uint1024_t& t, const uint1024_t& mod)
{
	uint1024_t ans(1), res(a), temp(t);
	while (temp != 0) {
		if (temp.Test(0))
			ans = MultiplyMod(ans, res, mod);
		res = MultiplyMod(res, res, mod);
		temp >>= 1;
	}
	return ans;
}
