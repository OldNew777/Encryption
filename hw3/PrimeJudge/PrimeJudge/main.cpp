#include <iostream>
#include <fstream>
#include <random>
#include <time.h>

#include "MillerRabin.h"
#include "uint512_t.h"

using namespace std;

int main() {
	srand(time(0));

	uint512_t a;
	a.Generate10_01();
	a &= (uint512_t(1) << 511) - (uint512_t(1) << 300);
	a.Set(0);

	a = 13;

	a.Print(stdout);
	printf("\n");

	if (MillerRabin::JudgePrime(a))
		cout << "Prime\n";
	else
		cout << "Not Prime\n";

	return 0;
}