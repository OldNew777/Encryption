#include <iostream>

using namespace std;

extern int crack();
extern int verify();

int main() {
	int order = -1;
	while (order != 1 && order != 2) {
		cout << "Input 1 to crack; Input 2 to verify. " << endl;
		cin >> order;
	}
	if (order == 1)
		return crack();
	else if (order == 2)
		return verify();
}