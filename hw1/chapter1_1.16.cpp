#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

int inverseSubstitution[9] = { 0, 2, 4, 6, 1, 8, 3, 5, 7 };

string decode(string ciphertext) {
	assert(ciphertext.length() % 8 == 0);
	string plaintext(ciphertext);
	for (int i = 0; i < ciphertext.length(); i += 8) {
		for (int j = 0; j < 8; j++)
			plaintext[i + j] = ciphertext[i + inverseSubstitution[j + 1] - 1];
	}
	return plaintext;
}

int main() {
	string ciphertext = "TGEEMNELNNTDROEOAAHDOETCSHAEIRLM";
	cout << decode(ciphertext) << endl;
}