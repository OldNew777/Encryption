#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

string shift_right(string str, int n) {
	assert(n >= 0);
	n = n % 26;
	for (int i = 0; i < str.length(); ++i) {
		str[i] = str[i] + n;
		if (str[i] > 'Z')
			str[i] -= 26;
	}
	return str;
}

int main() {
	string ciphertext = "BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD";
	for (int i = 0; i < 26; ++i)
		cout << i << " * shift right to decode. Plaintext :\t" << shift_right(ciphertext, i) << endl;
}