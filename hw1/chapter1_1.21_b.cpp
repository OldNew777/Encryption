#include <iostream>
#include <string>
#include <vector>

#include "Vigenere.h"

using namespace std;


int main() {
	string cipherText = ""
		"KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUD"
		"DKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYC"
		"QKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRL"
		"SVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMV"
		"GKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFS"
		"PEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHI"
		"FFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIY"
		"CWHJVLNHIQIBTKHJVNPIST";

	string subStr = Vigenere::FindMostxArySubString(cipherText, 3);
	cout << subStr << endl;

	int Kasiski_m = Vigenere::Kasiski_m(cipherText, subStr);
	cout << Kasiski_m << endl;

	vector<double> Ic = Vigenere::IndexOfCoincidenceVec(cipherText, Kasiski_m);
	cout << "\n**************** m = " << Kasiski_m << " ***************\n";
	for (int i = 0; i < Kasiski_m; ++i)
		cout << Ic[i] << endl;
	cout << "Average = " << Vigenere::Average(Ic) << "\n\n";

	vector<int> key;
	key.push_back(2);
	key.push_back(17);
	key.push_back(24);
	key.push_back(15);
	key.push_back(19);
	key.push_back(14);
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