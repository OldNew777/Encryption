#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include <algorithm>

#include "Frequency.h"

using namespace std;

char substitution[26];
char inversePermutation[26];

string decode(string ciphertext) {
	string plaintext = ciphertext;
	for (int i = 0; i < ciphertext.length(); ++i)
		plaintext[i] = inversePermutation[ciphertext[i] - 'A'];
	return plaintext;
}


void setSubstitution(char plainletter, char cipherletter) {
	substitution[plainletter - 'A'] = cipherletter;
	inversePermutation[cipherletter - 'A'] = plainletter;
}

int main() {
	string cipherText = ""
		"EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCK"
		"QPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSHYSXCG"
		"OIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZU"
		"GFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZGACGNFGLKNS"
		"ACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNC"
		"IACZEJNCSHFZEJZEGMXCYHCJUMGKUCY";
	memset(substitution, '-', 26);
	memset(inversePermutation, '-', 26);

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

	// ******************confirm key*******************
	setSubstitution('E', 'C');
	setSubstitution('W', 'F');
	setSubstitution('H', 'Z');
	setSubstitution('L', 'N');
	setSubstitution('T', 'U');
	setSubstitution('P', 'X');
	setSubstitution('O', 'S');
	setSubstitution('N', 'O');
	setSubstitution('B', 'D');
	setSubstitution('A', 'G');
	// ******************confirm key*******************

	// output
	int stride = 45;
	string plainText = decode(cipherText);
	cout << endl;
	for (int i = 0; i < cipherText.length(); i += stride) {
		int substrLen = min(stride, int(cipherText.length()) - i);
		cout << cipherText.substr(i, substrLen) << '\n';
		cout << plainText.substr(i, substrLen) << "\n";
		cout << endl;
	}
}