#ifndef KASISKI_H_
#define KASISKI_H_

#include <string>
#include <cmath>
#include <vector>
#include <assert.h>
#include <map>
#include "Frequency.h"


using namespace std;

class Vigenere {
private:
	static int __gcd(int a, int b) {
		if (b > a)
			swap(a, b);
		int t;
		while (b != 0) {
			t = a % b;
			a = b;
			b = t;
		}
		return a;
	}

	static vector<int> GetPosition(string cipherText, string subStr) {
		vector<int> position;
		int index = 0;
		while ((index = cipherText.find(subStr, index)) != cipherText.npos) {
			position.push_back(index);
			index += subStr.length();
		}
		return position;
	}

	static int Get_gcd(const vector<int>& nums) {
		assert(nums.size() >= 2);
		int gcd = nums[1] - nums[0];
		for (int i = 2; i < nums.size(); ++i) {
			int dist = nums[i] - nums[0];
			gcd = __gcd(gcd, dist);
		}
		return gcd;
	}

	static string Process(const string& text, const vector<int>& key, int flag) {
		string ans = text;
		int m = key.size();
		for (int i = 0; i < text.size(); ++i) {
			ans[i] = text[i] + flag * key[i % m];
			if (ans[i] < 'A')
				ans[i] += 26;
			else if (ans[i] > 'Z')
				ans[i] -= 26;
		}
		return ans;
	}

public:
	static string FindMostxArySubString(const string& text, int x) {
		map<string, int> subStrNum;
		string subStr = "-" + text.substr(0, x - 1);
		for (int i = x - 1; i < text.size() - x + 1; ++i) {
			subStr = subStr.substr(1, x - 1) + text[i];
			++subStrNum[subStr];
		}

		int max = -1;
		for (auto iter = subStrNum.begin(); iter != subStrNum.end(); ++iter)
			if (max < iter->second) {
				max = iter->second;
				subStr = iter->first;
			}

		if (PrintFlag)
			printf("Num  = %d, %s\n", max, subStr.c_str());

		return subStr;
	}

	static string Encode(const string& plainText, const vector<int>& key) {
		return Process(plainText, key, 1);
	}

	static string Decode(const string& cipherText, const vector<int>& key) {
		return Process(cipherText, key, -1);
	}

	static int Kasiski_m(const string& cipherText, string subStr) {
		vector<int> position = GetPosition(cipherText, subStr);
		int gcd = Get_gcd(position);
		return gcd;
	}

	static vector<string> Separate(const string& text, int m) {
		vector<string> str(m, "");
		for (int i = 0; i < text.size(); ++i)
			str[i % m] += text[i];
		return str;
	}

	static vector<double> IndexOfCoincidenceVec(const string& cipherText, int m) {
		vector<string> str = Separate(cipherText, m);
		vector<double> Ic(m);
		for (int i = 0; i < m; ++i) {
			if (PrintFlag)
				printf("i = %d ", i);
			Ic[i] = IndexOfCoincidence(str[i]);
		}
		return Ic;
	}

	static double IndexOfCoincidence(const string& text) {
		Frequency frequency[26];
		Frequency::GetFrequency(text, frequency);
		double Ic = 0;
		for (int i = 0; i < 26; ++i) {
			Ic += pow(frequency[i].frequency, 2);
		}

		if (PrintFlag) {
			printf("---------------------------------\n");
			for (int g = 0; g < 26; ++g) {
				double Mg = 0;
				for (int i = 0; i < 26; ++i)
					Mg += Frequency::GetSingleLetterFrequency('A' + i) * frequency[(i + g) % 26].frequency;
				printf("g = %d, Mg = %f\n", g, Mg);
			}
			printf("---------------------------------\n");
		}

		return Ic;
	}

	static double Average(const vector<double>& nums) {
		double average = 0;
		for (double num : nums)
			average += num;
		average /= nums.size();
		return average;
	}

	static bool PrintFlag;
};

bool Vigenere::PrintFlag = true;

#endif