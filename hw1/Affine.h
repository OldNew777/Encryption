#ifndef AFFINE_H_
#define AFFINE_H_

#include <string>
#include <assert.h>


using namespace std;

class Affine {
private:
	static int InverseElement(int a) {
		assert(a == 1 || a == 3 || a == 5 || a == 7 || a == 9 || a == 11 ||
			a == 15 || a == 17 || a == 19 || a == 21 || a == 23 || a == 25);
		switch (a) {
		case 1:
			return 1;
		case 3:
			return 9;
		case 9:
			return 3;
		case 5:
			return 21;
		case 21:
			return 5;
		case 7:
			return 15;
		case 15:
			return 7;
		case 11:
			return 19;
		case 19:
			return 11;
		case 17:
			return 23;
		case 23:
			return 17;
		case 25:
			return 25;
		}
		throw exception();
	}

public:
	static string Encode(const string& plainText, int a, int b) {
		assert(a == 1 || a == 3 || a == 5 || a == 7 || a == 9 || a == 11 || 
			a == 15 || a == 17 || a == 19 || a == 21 || a == 23 || a == 25);
		string cipherText = plainText;
		for (int i = 0; i < plainText.size(); ++i)
			cipherText[i] = (((plainText[i] - 'A') * a + b) % 26) + 'A';
		return cipherText;
	}

	static string Decode(const string& cipherText, int a, int b) {
		assert(a == 1 || a == 3 || a == 5 || a == 7 || a == 9 || a == 11 ||
			a == 15 || a == 17 || a == 19 || a == 21 || a == 23 || a == 25);
		string plainText = cipherText;
		for (int i = 0; i < cipherText.size(); ++i) {
			int f = (cipherText[i] - 'A') - b;
			while (f < 0)
				f += 26;
			while (f >= 26)
				f -= 26;
			plainText[i] = (InverseElement(a) * f % 26) + 'A';
		}
		return plainText;
	}
};

#endif