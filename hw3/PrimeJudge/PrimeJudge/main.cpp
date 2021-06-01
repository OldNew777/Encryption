#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <string>
#include <stdlib.h>

#include "MillerRabin.h"
#include "uint512_t.h"
#include "uint1024_t.h"

#define MAX_PRIME_NUM 2

using namespace std;

int RandMillerRabin() {
	bool reset = true;
	uint512_t a;
	for (int i = 0, j = 0; i < MAX_PRIME_NUM || j < MAX_PRIME_NUM;) {
		if (reset)
			a.Generate10_01();
		else {
			a += 2;
			a.Set(511);
		}

		bool prime = MillerRabin::JudgePrime(a);

		if (prime && i < MAX_PRIME_NUM) {
			cout << "Prime No." << i << "\n";
			a.Print(stdout);
			cout << endl;

			string name = "Prime_Bin_" + to_string(i) + ".in";
			FILE* file = fopen(name.c_str(), "w");
			a.Print(file);
			fclose(file);

			++i;
			reset = true;
		}
		else if (!prime && j < MAX_PRIME_NUM) {
			cout << "NotPrime No." << j << "\n";
			a.Print(stdout);
			cout << endl;

			string name = "NotPrime_Bin_" + to_string(j) + ".in";
			FILE* file = fopen(name.c_str(), "w");
			a.Print(file);
			fclose(file);

			++j;
			reset = true;
		}
		else
			reset = false;
	}

	string process = "python ./process.py " + to_string(MAX_PRIME_NUM);
	system(process.c_str());

	return 0;
}

int TestMillerRabin() {
	string prefix[2] = { "", "Not" };
	for (int p = 0; p < 2; ++p) {
		for (int i = 0; i < MAX_PRIME_NUM; ++i) {
			string name = prefix[p] + "Prime_Bin_" + to_string(i) + ".in";
			FILE* file = fopen(name.c_str(), "r");
			uint512_t a;
			a.Read(file);
			fclose(file);

			bool prime = MillerRabin::JudgePrime(a);
			if (prime ^ bool(p))
				cout << prefix[p] << "Prime_" << i << " case is right\n";
			else
				cout << prefix[p] << "Prime_" << i << " case is wrong\n";

			a.Print(stdout);
			printf("\n");
		}

	}

	return 0;
}

int main() {
	srand(time(0));

	//RandMillerRabin();

	TestMillerRabin();
}