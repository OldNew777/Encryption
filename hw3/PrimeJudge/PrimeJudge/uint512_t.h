#pragma once

#pragma warning(disable:4996)

#include <stdint.h>
#include <stdio.h>
#include <random>

class uint1024_t;

class uint512_t
{
	friend class uint1024_t;

private:
	uint64_t data[8];

public:
	uint512_t() {
		for (int i = 0; i < 8; ++i)
			data[i] = 0;
	}
	uint512_t(const uint64_t& t) {
		for (int i = 1; i < 8; ++i)
			data[i] = 0;
		data[0] = t;
	}
	uint512_t(const uint512_t& t) {
		for (int i = 0; i < 8; ++i)
			data[i] = t.data[i];
	}
	uint512_t(const uint1024_t& t);

	inline void Rand() {
		for (int i = 0; i < 8; ++i) {
			// rand() range in [0, 32767], namely 15bits
			// 64 = 15*4 + 4
			uint64_t d0 = rand(), d1 = rand(), d2 = rand(), d3 = rand(), d4 = rand();
			data[i] = (d0 << 49) | (d1 << 34) | (d2 << 19) | (d3 << 4) | (d4 & 0b1111);
		}
	}
	inline void Generate10_01() {
		Rand();
		Set(0);
		Set(511);
	}

	inline void Set(int index) {
		int block = index >> 6;
		index &= 63;
		uint64_t flag = uint64_t(1) << index;
		data[block] |= flag;
	}
	inline void Unset(int index) {
		int block = index >> 6;
		index &= 63;
		uint64_t flag = uint64_t(1) << index;
		data[block] &= ~flag;
	}
	inline bool Test(int index) const {
		int block = index >> 6;
		index &= 63;
		uint64_t flag = uint64_t(1) << index;
		return data[block] & flag;
	}


	// bits shift
	uint512_t operator << (uint32_t shiftBits) const {
		uint512_t ret(*this);
		if (shiftBits == 0)
			return ret;
		else if (shiftBits >= 512) {
			uint512_t zeros;
			return zeros;
		}

		int strideCarry = (shiftBits + 63) >> 6, strideRemaining = strideCarry - 1;
		uint32_t shiftBitsInBlock = shiftBits & 127;
		if (shiftBitsInBlock > 64)
			shiftBitsInBlock &= 63;
		else if (shiftBitsInBlock == 0)
			shiftBitsInBlock = 64;

		uint64_t flag = int64_t(0x8000000000000000) >> (shiftBitsInBlock - 1);
		uint64_t pData[16] = { 0 },
			* carry = pData + 8,
			* data = ret.data;
		for (int i = 7; i >= 0; --i) {
			carry[i] = (flag & data[i]) >> (64 - shiftBitsInBlock);
		}
		for (int i = 7; i >= 0; --i) {
			data[i] = ((i - strideRemaining >= 0) ? (data[i - strideRemaining] << shiftBitsInBlock) : 0) | carry[i - strideCarry];
		}
		return ret;
	}
	uint512_t operator >> (uint32_t shiftBits) const {
		uint512_t ret(*this);
		if (shiftBits == 0)
			return ret;
		else if (shiftBits >= 512) {
			uint512_t zeros;
			return zeros;
		}

		int strideCarry = (shiftBits + 63) >> 6, strideRemaining = strideCarry - 1;
		uint32_t shiftBitsInBlock = shiftBits & 127;
		if (shiftBitsInBlock > 64)
			shiftBitsInBlock &= 63;
		else if (shiftBitsInBlock == 0)
			shiftBitsInBlock = 64;

		uint64_t flag = int64_t(0x8000000000000000) >> (shiftBitsInBlock - 1);
		flag >>= (64 - shiftBitsInBlock);
		uint64_t pData[16] = { 0 },
			* carry = pData,
			* data = ret.data;
		for (int i = 0; i < 8; ++i) {
			carry[i] = (flag & data[i]) << (64 - shiftBitsInBlock);
		}
		for (int i = 0; i < 8; ++i) {
			data[i] = ((i + strideRemaining < 8) ? (data[i + strideRemaining] >> shiftBitsInBlock) : 0) | carry[i + strideCarry];
		}
		return ret;
	}
	uint512_t& operator <<= (uint32_t shiftBits) {
		if (shiftBits == 0)
			return *this;
		else if (shiftBits >= 512) {
			for (int i = 0; i < 8; ++i)
				data[i] = 0;
			return *this;
		}

		int strideCarry = (shiftBits + 63) >> 6, strideRemaining = strideCarry - 1;
		uint32_t shiftBitsInBlock = shiftBits & 127;
		if (shiftBitsInBlock > 64)
			shiftBitsInBlock &= 63;
		else if (shiftBitsInBlock == 0)
			shiftBitsInBlock = 64;

		uint64_t flag = int64_t(0x8000000000000000) >> (shiftBitsInBlock - 1);
		uint64_t pData[16] = { 0 },
			* carry = pData + 8,
			* data = this->data;
		for (int i = 7; i >= 0; --i) {
			carry[i] = (flag & data[i]) >> (64 - shiftBitsInBlock);
		}
		for (int i = 7; i >= 0; --i) {
			data[i] = ((i - strideRemaining >= 0) ? (data[i - strideRemaining] << shiftBitsInBlock) : 0) | carry[i - strideCarry];
		}
		return *this;
	}
	uint512_t& operator >>= (uint32_t shiftBits) {
		if (shiftBits == 0)
			return *this;
		else if (shiftBits >= 512) {
			for (int i = 0; i < 8; ++i)
				data[i] = 0;
			return *this;
		}

		int strideCarry = (shiftBits + 63) >> 6, strideRemaining = strideCarry - 1;
		uint32_t shiftBitsInBlock = shiftBits & 127;
		if (shiftBitsInBlock > 64)
			shiftBitsInBlock &= 63;
		else if (shiftBitsInBlock == 0)
			shiftBitsInBlock = 64;

		uint64_t flag = int64_t(0x8000000000000000) >> (shiftBitsInBlock - 1);
		flag >>= (64 - shiftBitsInBlock);
		uint64_t pData[16] = { 0 },
			* carry = pData,
			* data = this->data;
		for (int i = 0; i < 8; ++i) {
			carry[i] = (flag & data[i]) << (64 - shiftBitsInBlock);
		}
		for (int i = 0; i < 8; ++i) {
			data[i] = ((i + strideRemaining < 8) ? (data[i + strideRemaining] >> shiftBitsInBlock) : 0) | carry[i + strideCarry];
		}
		return *this;
	}

	// compare
	inline bool operator < (const uint512_t& t) const {
		for (int i = 7; i >= 0; --i) {
			if (data[i] < t.data[i])
				return true;
			if (data[i] > t.data[i])
				return false;
		}
		return false;
	}
	inline bool operator <= (const uint512_t& t) const {
		for (int i = 7; i >= 0; --i) {
			if (data[i] < t.data[i])
				return true;
			if (data[i] > t.data[i])
				return false;
		}
		return true;
	}
	inline bool operator > (const uint512_t& t) const {
		for (int i = 7; i >= 0; --i) {
			if (data[i] > t.data[i])
				return true;
			if (data[i] < t.data[i])
				return false;
		}
		return false;
	}
	inline bool operator >= (const uint512_t& t) const {
		for (int i = 7; i >= 0; --i) {
			if (data[i] > t.data[i])
				return true;
			if (data[i] < t.data[i])
				return false;
		}
		return true;
	}
	inline bool operator == (const uint512_t& t) const {
		for (int i = 7; i >= 0; --i) {
			if (data[i] != t.data[i])
				return false;
		}
		return true;
	}
	inline bool operator != (const uint512_t& t) const {
		for (int i = 7; i >= 0; --i) {
			if (data[i] != t.data[i])
				return true;
		}
		return false;
	}

	// operator with uint512_t
	inline uint512_t operator + (const uint512_t& t) const {
		uint512_t ret(*this);
		uint64_t carry = 0;
		for (int i = 0; i < 8; ++i) {
			uint64_t plus = t.data[i] + carry;
			carry = plus < t.data[i] ? 1 : 0;
			uint64_t ans = ret.data[i] + plus;
			carry += ans < ret.data[i] ? 1 : 0;
			ret.data[i] = ans;
		}
		return ret;
	}
	inline uint512_t operator - (const uint512_t& t) const {
		uint512_t ret(*this);
		uint64_t carry = 0;
		for (int i = 0; i < 8; ++i) {
			uint64_t minus = t.data[i] + carry;
			carry = minus < t.data[i] ? 1 : 0;
			uint64_t ans = ret.data[i] - minus;
			carry += ans > ret.data[i] ? 1 : 0;
			ret.data[i] = ans;
		}
		return ret;
	}
	inline uint512_t operator * (const uint512_t& t) const {
		uint512_t b(t);
		uint512_t ans(0), res(*this);
		while (b != 0) {
			if (b.Test(0))
				ans = ans + res;
			res <<= 1;
			b >>= 1;
		}
		return ans;
	}
	inline uint512_t operator / (const uint512_t& t) const {
		uint512_t ans, a(*this), b(t), temp, add = 1;
		while (b < a && !b.Test(511)) {
			temp = b;
			b <<= 1;
			add <<= 1;
		}
		if (b > a) {
			b = temp;
			add >>= 1;
		}
		while (a >= t) {
			if (a >= b) {
				a -= b;
				ans += add;
			}
			b >>= 1;
			add >>= 1;
		}
		return ans;
	}
	inline uint512_t operator % (const uint512_t& t) const {
		uint512_t a(*this), b(t), temp;
		while (b < a && !b.Test(511)) {
			temp = b;
			b <<= 1;
		}
		if (b > a) {
			b = temp;
		}
		while (a >= t) {
			if (a >= b) {
				a -= b;
			}
			b >>= 1;
		}
		return a;
	}
	inline uint512_t operator & (const uint512_t& t) const {
		uint512_t ret(*this);
		for (int i = 0; i < 8; ++i)
			ret.data[i] &= t.data[i];
		return ret;
	}
	inline uint512_t operator | (const uint512_t& t) const {
		uint512_t ret(*this);
		for (int i = 0; i < 8; ++i)
			ret.data[i] |= t.data[i];
		return ret;
	}
	inline uint512_t operator ^ (const uint512_t& t) const {
		uint512_t ret(*this);
		for (int i = 0; i < 8; ++i)
			ret.data[i] ^= t.data[i];
		return ret;
	}
	// & operator with uint512_t
	inline uint512_t& operator = (const uint512_t& t) {
		for (int i = 0; i < 8; ++i)
			data[i] = t.data[i];
		return *this;
	}
	inline uint512_t& operator += (const uint512_t& t) {
		uint64_t carry = 0;
		for (int i = 0; i < 8; ++i) {
			uint64_t plus = t.data[i] + carry;
			carry = plus < t.data[i] ? 1 : 0;
			uint64_t ans = data[i] + plus;
			carry += ans < data[i] ? 1 : 0;
			data[i] = ans;
		}
		return *this;
	}
	inline uint512_t& operator -= (const uint512_t& t) {
		uint64_t carry = 0;
		for (int i = 0; i < 8; ++i) {
			uint64_t minus = t.data[i] + carry;
			carry = minus < t.data[i] ? 1 : 0;
			uint64_t ans = data[i] - minus;
			carry += ans > data[i] ? 1 : 0;
			data[i] = ans;
		}
		return *this;
	}
	inline uint512_t& operator *= (const uint512_t& t) {
		uint512_t b(t);
		uint512_t ans(0);
		while (b != 0) {
			if (b.Test(0))
				ans = ans + *this;
			*this <<= 1;
			b >>= 1;
		}
		*this = ans;
		return *this;
	}
	inline uint512_t& operator /= (const uint512_t& t) {
		uint512_t ans, a(*this), b(t), temp, add = 1;
		while (b < a && !b.Test(511)) {
			temp = b;
			b <<= 1;
			add <<= 1;
		}
		if (b > a) {
			b = temp;
			add >>= 1;
		}
		while (a >= t) {
			if (a >= b) {
				a -= b;
				ans += add;
			}
			b >>= 1;
			add >>= 1;
		}
		*this = ans;
		return *this;
	}
	inline uint512_t& operator %= (const uint512_t& t) {
		uint512_t a(*this), b(t), temp;
		while (b < a && !b.Test(511)) {
			temp = b;
			b <<= 1;
		}
		if (b > a) {
			b = temp;
		}
		while (a >= t) {
			if (a >= b) {
				a -= b;
			}
			b >>= 1;
		}
		*this = a;
		return *this;
	}
	inline uint512_t& operator &= (const uint512_t& t) {
		for (int i = 0; i < 8; ++i)
			data[i] &= t.data[i];
		return *this;
	}
	inline uint512_t& operator |= (const uint512_t& t) {
		for (int i = 0; i < 8; ++i)
			data[i] |= t.data[i];
		return *this;
	}
	inline uint512_t& operator ^= (const uint512_t& t) {
		for (int i = 0; i < 8; ++i)
			data[i] ^= t.data[i];
		return *this;
	}

	// single operator
	inline bool operator ! () const {
		for (int i = 0; i < 8; ++i) {
			if (data[i] != 0)
				return false;
		}
		return true;
	}
	inline uint512_t operator ~ () const {
		uint512_t ret;
		for (int i = 0; i < 8; ++i) {
			ret.data[i] = ~data[i];
		}
		return ret;
	}

	inline uint512_t& operator ++ () {
		*this += 1;
		return *this;
	}
	inline uint512_t operator ++ (int) {
		uint512_t ret(*this);
		*this += 1;
		return ret;
	}
	inline uint512_t& operator -- () {
		*this -= 1;
		return *this;
	}
	inline uint512_t operator -- (int) {
		uint512_t ret(*this);
		*this -= 1;
		return ret;
	}

	void Print(FILE* _Stream, bool binary = false) const {
		const uint64_t* data = this->data;
		if (binary) {
			for (int i = 7; i >= 0; --i)
				fprintf(_Stream, "%llu", data[i]);
		}
		else {
			for (int i = 7; i >= 0; --i) {
				uint64_t flag = 0x8000000000000000;
				for (int j = 63; j >= 0; --j) {
					fprintf(_Stream, "%d", (data[i] & flag) ? 1 : 0);
					flag >>= 1;
					if (!(j & 7))
						fprintf(_Stream, " ");
				}
				fprintf(_Stream, "\n");
			}
		}
	}
	void Read(FILE* _Stream, bool binary = false) {
		uint64_t* data = this->data;
		for (int i = 0; i < 8; ++i)
			data[i] = 0;

		if (binary) {
			for (int i = 7; i >= 0; --i)
				if (fscanf(_Stream, "%llu", &data[i]) != 1)
					throw "Read uint512_t fail!";
		}
		else {
			for (int i = 7; i >= 0; --i) {
				uint64_t flag = 0x8000000000000000;
				for (int j = 63; j >= 0;) {
					char ch = '\0';
					if (fscanf(_Stream, "%c", &ch) != 1)
						throw "Read uint512_t fail!";
					if (ch == ' ' || ch == '\n' || ch == '\t')
						continue;
					if (ch == '1')
						data[i] |= flag;
					else if (ch != '0')
						throw "Read uint512_t fail. Text should be '0'/'1' !";
					flag >>= 1;
					--j;
				}
			}
		}
	}
};
