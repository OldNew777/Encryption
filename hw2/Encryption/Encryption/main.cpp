#include <iostream>
#include <stdint.h>
#include <time.h>
#include <omp.h>

#include "RC4.h"
#include "DataGenerator.h"

using namespace std;

void print(uint8_t* data, uint32_t dataLen) {
	for (uint32_t i = 0; i < dataLen; ++i)
		printf("%c", data[i]);
	printf("\n\n");
}

void Test_AES_128() {

}

void Test_RC4() {
	uint32_t dataSum_Mb = 100, keyLen = 128, dataLen = dataSum_Mb << 17;

	uint8_t* key = DataGenerator(128);
	uint8_t* data1 = DataGenerator(dataLen),
		* data2 = new uint8_t[dataLen];
	memcpy(data2, data1, dataLen);

	RC4 rc4;
	rc4.Init(key, keyLen);
	//rc4.PrintSBox();

	clock_t start = clock();
	rc4.Encrypt(data2, dataLen);
	clock_t end = clock();

	rc4.Init(key, keyLen);
	rc4.Encrypt(data2, dataLen);

	double speed = dataSum_Mb / double(end - start) * 1000;
	cout << (end - start) << "ms\n";
	if (memcmp(data1, data2, dataLen) == 0)
		cout << "RC4 Speed = " << speed << " Mbps\n";
	else
		cout << "RC4 Wrong answer!\n";

	delete[] key;
	delete[] data1;
	delete[] data2;
}

void Test_SHA_3_256() {

}

int main() {
	Test_AES_128();
	Test_RC4();
	Test_SHA_3_256();
	return 0;
}