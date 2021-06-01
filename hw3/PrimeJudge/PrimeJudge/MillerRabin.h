#pragma once

#include "uint1024_t.h"
#include "uint512_t.h"

class MillerRabin
{
private:
	static uint32_t Prime[50];

public:
	static bool JudgePrime(const uint512_t& n);
};


uint1024_t MultiplyMod(const uint1024_t& a, const uint1024_t& b, const uint1024_t& mod);
uint1024_t PowMod(const uint1024_t& a, const uint1024_t& t, const uint1024_t& mod);