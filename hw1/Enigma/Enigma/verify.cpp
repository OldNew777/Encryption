#include <iostream>
#include <string>
#include <assert.h>
#include <time.h>

#include "enigma.h"

using namespace std;

int verify() {
	char rotor_init_position[3] = { 'A', 'G', 'C' };
	int rotorSetting[3] = { 2, 1, 4 };
	int plugIn[26] = {
		24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14,
		10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19,
	};
	string plainText = ""
		"ASIOUGHAOSIUEGABNJDFXZKJQWERTYUIOPASDFGH"
		"JKLZXCVBNMQWERTYUIOPASDFGHJKLZXCVBNHMAIO";

	Enigma enigma(rotor_init_position, rotorSetting, plugIn);

	// œ» print enigma µƒ≈‰÷√
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