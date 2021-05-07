#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Frequency.h"
#include "Affine.h"

using namespace std;


int main() {
	string cipherText = ""
		"KQEREJEBCPPCJCRKIEACUZBKRVPKRBCIBQCARBJCVFCUP"
		"KRIOFKPACUZQEPBKRXPEIIEABDKPBCPFCDCCAFIEABDKP"
		"BCPFEQPKAZBKRHAIBKAPCCIBURCCDKDCCJCIDFUIXPAFF"
		"ERBICZDFKABICBBENEFCUPJCVKABPCYDCCDPKBCOCPERK"
		"IVKSCPICBRKIJPKABI";

	Frequency frequency[26], rightFrequency[26];
	Frequency::GetSingleLetterReal(rightFrequency);
	Frequency::GetFrequency(cipherText, frequency);

	sort(frequency, frequency + 26);
	sort(rightFrequency, rightFrequency + 26);

	for (int i = 0; i < 26; ++i) {
		rightFrequency[i].print();
		printf("\t");
		frequency[i].println();
	}

	int stride = 45;
	string plainText = Affine::Decode(cipherText, 19, 4);
	cout << endl;
	for (int i = 0; i < cipherText.length(); i += stride) {
		int substrLen = min(stride, int(cipherText.length()) - i);
		string ciphertext_phrase = cipherText.substr(i, substrLen), plaintext_phrase = plainText.substr(i, substrLen);
		cout << ciphertext_phrase << '\n';
		cout << plaintext_phrase << "\n\n";
	}
}