#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <time.h>
#include <random>

#include "enigma.h"

using namespace std;

int crack() {
	char rotor_init_position[3];
	int rotorSetting[3];
	int plugIn[26] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
	};

	// 随机初始化 enigma 的配置
	srand(time(0));
	int rotorSettingToBeSelected[5] = { 0,1,2,3,4 };
	for (int i = 0; i < 3; ++i) {
		rotor_init_position[i] = (rand() % 26) + 'A';
		int index = rand() % (5 - i);
		rotorSetting[i] = rotorSettingToBeSelected[index];
		for (int j = index; j < 5 - i - 1; ++j)
			rotorSettingToBeSelected[j] = rotorSettingToBeSelected[j + 1];
	}
	int plugIndex[26] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
	};
	// 随机化插线板配置
	int plugPairNum = rand() % 13;
	for (int i = 0; i < plugPairNum; ++i) {
		int index1 = rand() % (26 - 2 * i);
		int ch1 = plugIndex[index1];
		plugIndex[index1] = plugIndex[26 - 2 * i - 1];
		int index2 = rand() % (26 - 2 * i - 1);
		int ch2 = plugIndex[index2];
		plugIndex[index2] = plugIndex[26 - 2 * i - 2];
		plugIn[ch1] = ch2;
		plugIn[ch2] = ch1;
	}
	// 随机找一段明密文对
	int length = 80;
	string plainText(length, '*');
	for (int i = 0; i < length; ++i)
		plainText[i] = (rand() % 26) + 'A';

	Enigma enigma(rotor_init_position, rotorSetting, plugIn);

	// 先打印 enigma 的配置
	printf("Real enigma settings: \n");
	enigma.PrintKey();

	string cipherText = enigma.Encryption(plainText);

	cout << plainText << '\n';
	cout << cipherText << "\n\n";

	clock_t start = clock();
	Enigma::Crack(plainText, cipherText);
	double timeTotal = double(clock() - start) / CLOCKS_PER_SEC;
	cout << "Time = " << timeTotal << " s\n";

	return 0;
}