#include "uint1024_t.h"
#include "uint512_t.h"

uint1024_t::uint1024_t(const uint512_t& t)
{
	for (int i = 0; i < 8; ++i)
		data[i] = t.data[i];
	for (int i = 8; i < 16; ++i)
		data[i] = 0;
}
