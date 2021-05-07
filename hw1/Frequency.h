#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include <string>
#include <assert.h>

using namespace std;

class Frequency {
public:
	string letter;
	double frequency;
	int num = 0;

	bool operator<(const Frequency& other) const {
		return frequency < other.frequency;
	}
	void print() const {
		printf("%s : %f", letter.c_str(), frequency);
	}
	void println() const {
		printf("%s : %f\n", letter.c_str(), frequency);
	}

	static void GetFrequency(const string& text, Frequency single[26]) {
		for (int i = 0; i < 26; ++i) {
			single[i].letter = char('A' + i);
			single[i].num = 0;
		}

		int len = text.length();
		for (int i = 0; i < len; ++i)
			single[text[i] - 'A'].num += 1;

		for (int i = 0; i < 26; ++i)
			single[i].frequency = double(single[i].num) / len;
	}

	static void GetSingleLetterReal(Frequency dst[26]) {
		for (int i = 0; i < 26; ++i)
			dst[i] = singleLetterReal[i];
	}

	static double GetSingleLetterFrequency(char letter) {
		assert(letter >= 'A' && letter <= 'Z');
		return singleLetterReal[letter - 'A'].frequency;
	}

private:
	static Frequency singleLetterReal[26];
};

Frequency Frequency::singleLetterReal[26] = {
		{"A", 0.082},
		{"B", 0.015},
		{"C", 0.028},
		{"D", 0.043},
		{"E", 0.127},
		{"F", 0.022},
		{"G", 0.020},
		{"H", 0.061},
		{"I", 0.070},
		{"J", 0.002},
		{"K", 0.008},
		{"L", 0.040},
		{"M", 0.024},
		{"N", 0.067},
		{"O", 0.075},
		{"P", 0.019},
		{"Q", 0.001},
		{"R", 0.060},
		{"S", 0.063},
		{"T", 0.091},
		{"U", 0.028},
		{"V", 0.010},
		{"W", 0.023},
		{"X", 0.001},
		{"Y", 0.020},
		{"Z", 0.001},
};

#endif