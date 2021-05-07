#include <iostream>
#include <string>
#include <vector>

#include "Vigenere.h"

using namespace std;


int main() {
	string cipherText = ""
		"BNVSNSIHQCEELSSKKYERIFJKXUMBGYKAMQLJTYAVFBKVT"
		"DVBPVVRJYYLAOKYMPQSCGDLFSRLLPROYGESEBUUALRWXM"
		"MASAZLGLEDFJBZAVVPXWICGJXASCBYEHOSNMULKCEAHTQ"
		"OKMFLEBKFXLRRFDTZXCIWBJSICBGAWDVYDHAVFJXZIBKC"
		"GJIWEAHTTOEWTUHKRQVVRGZBXYIREMMASCSPBHLHJMBLR"
		"FFJELHWEYLWISTFVVYEJCMHYUYRUFSFMGESIGRLWALSWM"
		"NUHSIMYYITCCQPZSICEHBCCMZFEGVJYOCDEMMPGHVAAUM"
		"ELCMOEHVLTIPSUYILVGFLMVWDVYDBTHFRAYISYSGKVSUU"
		"HYHGGCKTMBLRX";

	// Vigenere::PrintFlag = false;

	for (int Kasiski_m = 6; Kasiski_m <= 6; ++Kasiski_m) {
		vector<double> Ic = Vigenere::IndexOfCoincidenceVec(cipherText, Kasiski_m);
		cout << "\n**************** m = " << Kasiski_m << " ***************\n";
		/*for (int i = 0; i < Kasiski_m; ++i)
			cout << Ic[i] << endl;*/
		cout << "Average = " << Vigenere::Average(Ic) << "\n";

	}

	vector<int> key;
	key.push_back(19);
	key.push_back(7);
	key.push_back(4);
	key.push_back(14);
	key.push_back(17);
	key.push_back(24);
	string plainText = Vigenere::Decode(cipherText, key);

	int stride = 45;
	for (int i = 0; i < plainText.length(); i += stride) {
		int substrLen = min(stride, int(cipherText.length()) - i);
		string plainText_phrase = plainText.substr(i, substrLen);
		string cipherText_phrase = cipherText.substr(i, substrLen);
		cout << cipherText_phrase << endl;
		cout << plainText_phrase << endl << endl;
	}
}