#include "uint512_t.h"
#include "uint1024_t.h"

uint512_t::uint512_t(const uint1024_t& t)
{
	for (int i = 0; i < 8; ++i)
		data[i] = t.data[i];
}
