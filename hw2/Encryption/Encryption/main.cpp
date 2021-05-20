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

void TestSpeed_AES_128() {
	uint32_t dataSum_Mb = 100, keyLen = 16, plainLen = dataSum_Mb << 17, cipherLen;

	uint8_t
		* key = DataGenerator(keyLen),
		* IV = DataGenerator(keyLen),
		* plainData = DataGenerator(plainLen);

	// 运行并计时
	clock_t start = clock();
	uint8_t* cipherData = AES_128::Encrypt(plainData, plainLen, cipherLen, key, IV);
	clock_t end = clock();
	double speedEncrypt = dataSum_Mb * 1000 / double(end - start);

	uint32_t plainLen2;
	start = clock();
	uint8_t* plainData2 = AES_128::Decrypt(cipherData, cipherLen, plainLen2, key, IV);
	end = clock();
	delete[] cipherData;
	double speedDecrypt = dataSum_Mb * 1000 / double(end - start);

	// 速率
	cout << "AES_128 Encrypt Speed = " << speedEncrypt << " Mbps\n";
	cout << "AES_128 Decrypt Speed = " << speedDecrypt << " Mbps\n\n";

	delete[] key;
	delete[] IV;
	delete[] plainData;
}

void Process16Kb_AES_128() {
	uint32_t dataSum_Kb = 16, keyLen = 16, plainLen = dataSum_Kb << 7, cipherLen;

	uint8_t
		* key = DataGenerator(keyLen),
		* IV = DataGenerator(keyLen),
		* plainData = DataGenerator(plainLen);

	// 导出IV
	ofstream ivOut("AES_128_IV.in", ios::out | ios::binary);
	ivOut.write((char*)IV, 16);
	ivOut.close();

	// 导出源数据
	ofstream plainOut("AES_128.in", ios::out | ios::binary);
	plainOut.write((char*)plainData, plainLen);
	plainOut.close();

	// 运行
	uint8_t* cipherData = AES_128::Encrypt(plainData, plainLen, cipherLen, key, IV);

	// 导出加密数据
	ofstream cipherOut("AES_128.out", ios::out | ios::binary);
	cipherOut.write((char*)cipherData, cipherLen);
	cipherOut.close();

	uint32_t plainLen2;
	uint8_t* plainData2 = AES_128::Decrypt(cipherData, cipherLen, plainLen2, key, IV);

	if (plainLen == plainLen2 && memcmp(plainData, plainData2, plainLen) == 0)
		cout << "AES_128 encrypted 16Kb data!\n\n";
	else
		cout << "AES_128 Wrong answer!\n\n";

	delete[] key;
	delete[] IV;
	delete[] cipherData;
	delete[] plainData;
	delete[] plainData2;
}

void Verify_AES_128() {
	uint32_t plainLen = 16, keyLen = 16, cipherLen;

	uint8_t
		* key = new uint8_t[keyLen],
		* IV = new uint8_t[16],
		* plainData = new uint8_t[plainLen];

	// DEBUG_ENCRYPT
	memset(key, 0, keyLen);
	memset(IV, 0, 16);
	uint8_t plainDataPreset[16] = {
		0x00, 0x00, 0x01, 0x01,
		0x03, 0x03, 0x07, 0x07,
		0x0f, 0x0f, 0x1f, 0x1f,
		0x3f, 0x3f, 0x7f, 0x7f,
	};
	memcpy(plainData, plainDataPreset, 16);

	// 运行
	uint8_t* cipherData = AES_128::Encrypt(plainData, plainLen, cipherLen, key, IV);
	uint8_t cipherDataRight[32] = {
		0xc7, 0xd1, 0x24, 0x19,
		0x48, 0x9e, 0x3b, 0x62,
		0x33, 0xa2, 0xc5, 0xa7,
		0xf4, 0x56, 0x31, 0x72,
		0xc4, 0x60, 0xcf, 0xc5,
		0x63, 0xe4, 0xab, 0xe9,
		0x51, 0x6e, 0x87, 0xcd,
		0x48, 0x4f, 0xb5, 0x60,
	};


	uint32_t plainLen2;
	uint8_t* plainData2 = AES_128::Decrypt(cipherData, cipherLen, plainLen2, key, IV);

	printf("AES test");
	printf("Initial key = \n");
	MathFunction::PrintHex(stdout, key, keyLen, true);
	printf("IV = \n");
	MathFunction::PrintHex(stdout, IV, 16, true);
	printf("PlainText = \n");
	MathFunction::PrintHex(stdout, plainData, plainLen, true);
	printf("EncodedText = \n");
	MathFunction::PrintHex(stdout, cipherData, cipherLen, true);
	printf("DecodedText = \n");
	MathFunction::PrintHex(stdout, plainData2, plainLen, true);

	if (plainLen == plainLen2 && memcmp(plainData, plainData2, plainLen) == 0 &&
		cipherLen == 32 && memcmp(cipherData, cipherDataRight, cipherLen) == 0)
		cout << "AES_128 Right answer!\n\n";
	else
		cout << "AES_128 Wrong answer!\n\n";

	delete[] key;
	delete[] IV;
	delete[] plainData;
	delete[] cipherData;
	delete[] plainData2;
}

void TestSpeed_RC4() {
	uint32_t dataSum_Mb = 100, keyLen = 128, dataLen = dataSum_Mb << 17;

	uint8_t* key = DataGenerator(keyLen);

	RC4 rc4;
	rc4.Init(key, keyLen);

	// 运行并计时
	clock_t start = clock();
	uint8_t* streamKey = rc4.GenerateStreamKey(dataLen);
	clock_t end = clock();

	// 计算速率
	double speed = dataSum_Mb * 1000 / double(end - start);
	cout << "RC4 Speed = " << speed << " Mbps\n\n";

	delete[] key;
	delete[] streamKey;
}

void Process16Kb_RC4() {
	uint32_t dataSum_Kb = 16, keyLen = 128, dataLen = dataSum_Kb << 7;

	uint8_t
		* key = DataGenerator(keyLen),
		* data1 = DataGenerator(dataLen),
		* data2 = new uint8_t[dataLen];
	memcpy(data2, data1, dataLen);

	// 导出源数据
	ofstream initKeyOut("RC4_initKey.in", ios::out | ios::binary);
	initKeyOut.write((char*)key, keyLen);
	initKeyOut.close();

	// 导出源数据
	ofstream plainOut("RC4.in", ios::out | ios::binary);
	plainOut.write((char*)data1, dataLen);
	plainOut.close();

	RC4 rc4;
	rc4.Init(key, keyLen);

	// 运行
	uint8_t* streamKey = rc4.GenerateStreamKey(dataLen);

	for (int i = 0; i < dataLen; ++i)
		data2[i] ^= streamKey[i];

	// 导出流密钥
	ofstream keyOut("RC4_key.out", ios::out | ios::binary);
	keyOut.write((char*)streamKey, dataLen);
	keyOut.close();

	// 导出加密数据
	ofstream cipherOut("RC4.out", ios::out | ios::binary);
	cipherOut.write((char*)data2, dataLen);
	cipherOut.close();

	cout << "RC4 produced 16Kb stream key, and encrypted 16Kb data!\n\n";

	delete[] key;
	delete[] streamKey;
	delete[] data1;
	delete[] data2;
}

void Verify_RC4() {
	uint32_t dataLen = 256, keyLen = 128;

	uint8_t
		key[] = "\'$kNntu~E,I9{,U<M\"lFXXI!K-cH}gP$sWfh*,hk0{>apfE(3uYM8[^MV,C@klm6$zTO7H_z1 I*(5Cq6j~.O1kZVEe(rzVuD!T;XV#R$+~uH{<2uGq<Sv9l2v+f8$BP",
		data[] = "(>gI5 XIgN`eH;Dj;f,ae;$bjydx4y~!(Wf N\'<\'pY(;fS\"\\%PTPDZ86(8P+g@W>Y37Zc1Q`HH[c&v\'^5QEpd5?=U+229lO]NcLQ1Rqt+ECs.HO1L0sD\'9{\"\")Hxl~I/4+Tg?4~@-2;6h#TKq4-5~t-[{#B\\j67?e~8zME6s-+\"y`bo.3nkJoy2Y4H-7N /jK,I?2npX9\\ nZ24faRKUwHXQI*^G-?`8BrGinOa^Fp^;X4l7.EMvx..-O0WL9Z^b";

	printf("RC4 Key (plain) = \n");
	MathFunction::PrintString(stdout, key, keyLen, false);
	printf("\n\n");
	printf("PlainData (plain) = \n");
	MathFunction::PrintString(stdout, data, dataLen, false);
	printf("\n\n");

	RC4 rc4;
	rc4.Init(key, keyLen);
	rc4.Encrypt(data, dataLen);

	printf("EncodedData (Hex) = \n");
	MathFunction::PrintHex(stdout, data, dataLen, false);
	printf("\n\n");

	uint8_t right[256];
	ifstream rightIn("RC4_caseright.out", ios::in | ios::binary);
	rightIn.read((char*)right, dataLen);
	rightIn.close();

	printf("Right (Hex) = \n");
	MathFunction::PrintHex(stdout, right, dataLen, false);
	printf("\n\n");

	if (memcmp(right, data, dataLen) == 0)
		cout << "RC4 Right answer!\n\n";
	else
		cout << "RC4 Wrong answer!\n\n";
}

void TestSpeed_SHA_3_256() {
	uint32_t dataSum_Mb = 100, dataLen = dataSum_Mb << 17;

	uint8_t* data = DataGenerator(dataLen);

	SHA_3_256 sha;
	uint8_t hash[HASH_LEN];

	// 运行并计时
	clock_t start = clock();
	sha.GetHash(data, dataLen, hash);
	clock_t end = clock();

	// 计算速率
	double speed = dataSum_Mb * 1000 / double(end - start);
	cout << "SHA_3_256 Speed = " << speed << " Mbps\n\n";

	delete[] data;
}

void Process16Kb_SHA_3_256() {
	uint32_t dataSum_Kb = 16, dataLen = dataSum_Kb << 7;

	uint8_t* data = DataGenerator(dataLen);

	// 导出源数据
	ofstream plainOut("SHA_3_256.in", ios::out | ios::binary);
	plainOut.write((char*)data, dataLen);
	plainOut.close();

	SHA_3_256 sha;
	uint8_t hash[HASH_LEN];

	// 运行
	sha.GetHash(data, dataLen, hash);

	// 导出hash数据
	FILE* hashOut = fopen("SHA_3_256.out", "w");
	MathFunction::PrintHex(hashOut, hash, HASH_LEN);
	fclose(hashOut);

	cout << "SHA_3_256 hashed 16Kb data!\n\n";

	delete[] data;
}

void Verify_SHA_3_256() {
	system("python check.py > SHA_3_256_right.out");

	// 比较
	uint8_t hashText[HASH_LEN << 1], hashRightText[HASH_LEN << 1];
	ifstream rightIn("SHA_3_256_right.out", ios::in);
	rightIn.read((char*)hashRightText, HASH_LEN << 1);
	rightIn.close();
	ifstream fileIn("SHA_3_256.out", ios::in);
	fileIn.read((char*)hashText, HASH_LEN << 1);
	fileIn.close();

	printf("My SHA_3_256 hash value = \n");
	MathFunction::PrintHex(stdout, hashText, HASH_LEN << 1, true);
	printf("\n");
	printf("Python hashlib.SHA3_256 hash value = \n");
	MathFunction::PrintHex(stdout, hashRightText, HASH_LEN << 1, true);
	printf("\n");

	if (memcmp(hashText, hashRightText, HASH_LEN) == 0)
		cout << "SHA3-256 Right answer!\n\n";
	else
		cout << "SHA3-256 Wrong answer!\n\n";
}

int main() {
	//// 预处理并打表输出MutGf结果
	//MathFunction::MutGfPresetInit();

	// 打印IV、Padding信息的flag
	AES_128::PrintFlag = true;

	// 测速
	printf("--------------- Test speed ---------------\n");
	TestSpeed_AES_128();
	printf("------------------------------------------\n");
	TestSpeed_RC4();
	printf("------------------------------------------\n");
	TestSpeed_SHA_3_256();
	printf("------------------------------------------\n");

	printf("\n");

	// 按题意处理16Kb数据
	printf("--------------- Process and output 16Kb data ---------------\n");
	Process16Kb_AES_128();
	printf("------------------------------------------------------------\n");
	Process16Kb_RC4();
	printf("------------------------------------------------------------\n");
	Process16Kb_SHA_3_256();    // 包含python的正确性测试
	printf("------------------------------------------------------------\n");

	printf("\n\n");

	// 正确性测试样例
	printf("--------------- My case ---------------\n");
	Verify_AES_128();
	printf("---------------------------------------\n");
	Verify_RC4();
	printf("---------------------------------------\n");
	Verify_SHA_3_256();
	printf("---------------------------------------\n");

	return 0;
}