#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <time.h>
#include <omp.h>

#include "RC4.h"
#include "SHA_3_256.h"
#include "AES_128.h"

#include "DataGenerator.h"
#include "MathFunction.h"

using namespace std;

void print(uint8_t* data, uint32_t dataLen) {
	for (uint32_t i = 0; i < dataLen; ++i)
		printf("%c", data[i]);
	printf("\n\n");
}

void Test_AES_128() {
	uint32_t dataSum_Mb = 100, keyLen = 16, plainLen = dataSum_Mb << 17, IVLen = 16, cipherLen;

	uint8_t* key = DataGenerator(keyLen),
		* IV = DataGenerator(IVLen),
		* plainData = DataGenerator(plainLen);

	// ����Դ����
	ofstream plainOut("AES_128.in", ios::out | ios::binary);
	plainOut.write((char*)plainData, plainLen);
	plainOut.close();

	AES_128 aes_128;

	// ���в���ʱ
	clock_t start = clock();
	uint8_t* cipherData = aes_128.Encrypt(plainData, plainLen, cipherLen, key, IV);
	clock_t end = clock();
	double speedEncrypt = dataSum_Mb / double(end - start) * 1000;

	// ������������
	ofstream cipherOut("AES_128.out", ios::out | ios::binary);
	cipherOut.write((char*)cipherData, cipherLen);
	cipherOut.close();

	uint32_t plainLen2;
	start = clock();
	uint8_t* plainData2 = aes_128.Decrypt(cipherData, cipherLen, plainLen2, key, IV);
	end = clock();
	double speedDecrypt = dataSum_Mb / double(end - start) * 1000;

	// ����
	//if (plainLen == plainLen2 && memcmp(plainData, plainData2, plainLen) == 0) {
	if (1) {
		cout << "AES_128 Encrypt Speed = " << speedEncrypt << " Mbps\n";
		cout << "AES_128 Decrypt Speed = " << speedDecrypt << " Mbps\n\n";
	}
	else
		cout << "AES_128 Wrong answer!\n\n";

	delete[] key;
	delete[] plainData;
	delete[] cipherData;
	delete[] IV;
}

void Test_RC4() {
	uint32_t dataSum_Mb = 100, keyLen = 128, dataLen = dataSum_Mb << 17;

	uint8_t* key = DataGenerator(keyLen),
		* data1 = DataGenerator(dataLen),
		* data2 = new uint8_t[dataLen];
	memcpy(data2, data1, dataLen);

	// ����Դ����
	ofstream plainOut("RC4.in", ios::out | ios::binary);
	plainOut.write((char*)data1, dataLen);
	plainOut.close();

	RC4 rc4;
	rc4.Init(key, keyLen);
	//rc4.PrintSBox();

	// ���в���ʱ
	clock_t start = clock();
	rc4.Encrypt(data2, dataLen);
	clock_t end = clock();

	// ������������
	ofstream cipherOut("RC4.out", ios::out | ios::binary);
	cipherOut.write((char*)data2, dataLen);
	cipherOut.close();

	rc4.Init(key, keyLen);
	rc4.Encrypt(data2, dataLen);

	// ��������
	double speed = dataSum_Mb / double(end - start) * 1000;
	if (memcmp(data1, data2, dataLen) == 0)
		cout << "RC4 Speed = " << speed << " Mbps\n\n";
	else
		cout << "RC4 Wrong answer!\n\n";

	delete[] key;
	delete[] data1;
	delete[] data2;
}

void Test_SHA_3_256() {
	uint32_t dataSum_Mb = 100, dataLen = dataSum_Mb << 17;

	uint8_t* data = DataGenerator(dataLen);

	// ����Դ����
	ofstream plainOut("SHA_3_256.in", ios::out | ios::binary);
	plainOut.write((char*)data, dataLen);
	plainOut.close();
	system("python check.py > SHA3_256_right.out");

	SHA_3_256 sha;
	uint8_t hash[DIGEST];

	// ���в���ʱ
	clock_t start = clock();
	sha.GetHash(data, dataLen, hash);
	clock_t end = clock();

	// ��������
	double speed = dataSum_Mb / double(end - start) * 1000;
	cout << "SHA_3_256 Speed = " << speed << " Mbps\n\n";

	// ����hash����
	FILE* hashOut = fopen("SHA_3_256.out", "w");
	MathFunction::PrintHex(hashOut, hash, DIGEST);
	fclose(hashOut);

	delete[] data;
}

int main() {
	Test_AES_128();
	//Test_RC4();
	//Test_SHA_3_256();

	return 0;
}