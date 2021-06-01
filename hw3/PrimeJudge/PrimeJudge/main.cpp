#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <string>

#include "MillerRabin.h"
#include "uint512_t.h"
#include "uint1024_t.h"

#define MAX_PRIME_NUM 10

using namespace std;

int RandMillerRabin() {
	uint512_t a;
	a.Generate10_01();
	a &= (uint512_t(1) << 511) - (uint512_t(1) << 400);
	a.Set(511);
	a.Set(0);

	for (int i = 0; i < MAX_PRIME_NUM;) {
		a += uint512_t(1) << 400;
		a.Set(511);

		//a.Print(stdout);
		//printf("\n");

		bool prime = MillerRabin::JudgePrime(a);

		if (prime) {
			cout << "No." << i << " is Prime\n";
			a.Print(stdout);
			string name = to_string(i) + ".in";
			FILE* file = fopen(name.c_str(), "w");
			a.Print(file);
			fclose(file);

			++i;
		}
		else {
			//cout << "No." << i << " is Prime\n";
		}
	}

	system("python ./process.py");

	return 0;
}

int TestMillerRabin() {
	for (int i = 0; i < MAX_PRIME_NUM; ++i) {
		string name = to_string(i) + ".in";
		FILE* file = fopen(name.c_str(), "r");
		uint512_t a;
		a.Read(file);
		fclose(file);

		bool prime = MillerRabin::JudgePrime(a);
		if (prime) {
			cout << "No." << i << " is Prime\n";
		}
		else {
			cout << "No." << i << " is not Prime\n";
		}
	}

	return 0;
}

int main() {
	srand(time(0));

	//RandMillerRabin();

	TestMillerRabin();
}