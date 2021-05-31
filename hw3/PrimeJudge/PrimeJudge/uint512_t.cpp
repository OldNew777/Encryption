#include "uint512_t.h"

uint512_t MultiplyMod(const uint512_t& a, const uint512_t& b, const uint512_t& mod)
{
	uint512_t temp(b);
	uint512_t ans(0), res(a);
	while (temp != 0) {
		if (temp.Test(0))
			ans = (ans + res) % mod;
		res <<= 1;
		temp >>= 1;
	}
	return ans;
}

uint512_t PowMod(const uint512_t& a, const uint512_t& t, const uint512_t& mod)
{
	uint512_t ans(1), res(a), temp(t);
	while (temp != 0) {
		if (temp.Test(0))
			ans = MultiplyMod(ans, res, mod);
		res = MultiplyMod(res, res, mod);
		temp >>= 1;
	}
	return ans;
}
