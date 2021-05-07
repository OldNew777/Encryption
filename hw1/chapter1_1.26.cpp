#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

string decode(string ciphertext, int m, int n) {
	string plaintext = ciphertext;
	for (int k = 0; k < ciphertext.length(); k += m * n)
		for (int i = 0; i < m; ++i)
			for (int j = 0; j < n; ++j)
				plaintext[k + i * n + j] = ciphertext[k + j * m + i];
	return plaintext;
}

int main() {
	string ciphertext = "MYAMRARUYIQTENCTORAHROYWDSOYEOUARRGDERNOGW";
	for (int m = 2; m < ciphertext.length(); ++m)
		if (ciphertext.length() % m == 0)
			for (int n = 2; n <= ciphertext.length() / m; ++n)
				if ((ciphertext.length() / m) % n == 0) {
					printf("(m, n) = (%d, %d)  \t", m, n);
					cout << decode(ciphertext, m, n) << endl;
				}
}