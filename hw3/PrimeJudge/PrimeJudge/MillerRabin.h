#pragma once

#include "uint512_t.h"

class MillerRabin
{
private:
	static uint32_t Prime[50];

public:
	static bool JudgePrime(const uint512_t& n);
};
