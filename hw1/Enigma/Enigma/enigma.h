#ifndef ENIGMA_H_
#define ENIGMA_H_

#include <string>
#include <algorithm>
#include <queue>
#include <assert.h>

#include "v.h"

class Enigma {
private:
	// 固定的rotor、reflector、trigger信息
	static int ReflectorPreset[2][26];
	static int RotorSettingPreset[5][26];
	static int RotorTrigger[5];

	int _rotorLetter[3];
	int _rotorTrigger[3];
	int* _rotor[3];
	int* _reflector;
	int _plugIn[26];
	bool _plugInChangable[26];

	// 记录初始化信息
	int _rotorInitPosition[3];
	int _rotorSetting[3];

	void init(char rotor_init_position[3], int rotorSetting[3], int plugIn[26]) {
		for (int i = 0; i < 26; ++i) {
			_plugIn[i] = plugIn[i];
			_plugInChangable[i] = true;
		}

		for (int i = 0; i < 26; ++i) {
			if (_plugIn[_plugIn[i]] != i)
				throw std::exception();
		}

		_reflector = &(ReflectorPreset[0][0]);

		for (int i = 0; i < 3; ++i) {
			_rotorLetter[i] = rotor_init_position[i] - 'A';
			_rotor[i] = &(RotorSettingPreset[rotorSetting[i]][0]);
			_rotorTrigger[i] = RotorTrigger[rotorSetting[i]];

			_rotorInitPosition[i] = rotor_init_position[i] - 'A';
			_rotorSetting[i] = rotorSetting[i];
		}

	}

	void rotorRotate(int rotorN) {
		if (rotorN == 2) {
			_rotorLetter[2] = (_rotorLetter[2] + 1) % 26;
			rotorRotate(1);
		}
		else if ((rotorN == 1 || rotorN == 0) && _rotorLetter[rotorN + 1] == _rotorTrigger[rotorN + 1]) {
			_rotorLetter[rotorN] = (_rotorLetter[rotorN] + 1) % 26;
			rotorRotate(rotorN - 1);
		}
		else
			return;

		if (PrintFlag_Encryption)
			printf("Rotors Position : %c%c%c\n",
				to_char(_rotorLetter[0]),
				to_char(_rotorLetter[1]),
				to_char(_rotorLetter[2])
			);
	}

	inline int find(int target, int arr[26]) const {
		for (int i = 0; i < 26; ++i)
			if (arr[i] == target) {
				if (PrintFlag_Encryption)
					printf("### find %d : %d\n", target, i);

				return i;
			}
		return -1;
	}

	static char to_char(int text) {
		return text + 'A';
	}

	static vector<Enigma> TestPlugIn(
		const vector<vector<V>>& rings, Enigma enigma_f,
		Enigma& enigma_NoPlug,
		queue<int> testPlugQueue,
		const vector<vector<pair<int, int>>>& ringsInfo) {

		int repeatTimes = 0;
		while (testPlugQueue.size() > 0) {
			int testPlugInt = testPlugQueue.front();
			testPlugQueue.pop();

			char testPlugChar = to_char(testPlugInt);
			set<int> posiblePlug = {
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
				13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
			};

			// 符合ring的可能解
			for (int ringsInfoIndex = 0; ringsInfoIndex < ringsInfo[testPlugInt].size(); ++ringsInfoIndex) {
				int i = ringsInfo[testPlugInt][ringsInfoIndex].first;
				int startPosition = ringsInfo[testPlugInt][ringsInfoIndex].second;

				set<int> posiblePlug_Ring;
				for (char k = 'A'; k <= 'Z'; ++k) {
					char text = k;
					for (int j = startPosition; j < startPosition + rings[i].size(); ++j) {
						enigma_NoPlug.resetRotorPosition();
						enigma_NoPlug.rotateTimes(rings[i][j % rings[i].size()].index);
						text = enigma_NoPlug.Encryption(text);
					}
					if (text == k)
						posiblePlug_Ring.insert(k - 'A');
				}
				set<int> imposiblePlug;
				for (int j : posiblePlug)
					if (posiblePlug_Ring.find(j) == posiblePlug_Ring.end())
						imposiblePlug.insert(j);
				for (int j : imposiblePlug)
					posiblePlug.erase(j);
				if (posiblePlug.size() == 0)
					break;
			}

			// plug对还必须与前面检查出来的plug配置不冲突
			if (enigma_f.addPlugIn(testPlugChar, testPlugChar)) {
				// 还没有plug对
				set<int> posiblePlug_tmp;
				for (int i : posiblePlug)
					if (enigma_f.addPlugIn(to_char(i), testPlugChar)) {
						posiblePlug_tmp.insert(i);
						enigma_f.removePlugIn(to_char(i), testPlugChar);
					}
				posiblePlug = posiblePlug_tmp;
			}
			else {
				// 已经有了plug对
				set<int> posiblePlug_tmp;
				for (int i : posiblePlug)
					if (enigma_f.addPlugIn(to_char(i), testPlugChar)) {
						posiblePlug_tmp.insert(i);
						break;
					}
				posiblePlug = posiblePlug_tmp;
			}

			// 不满足条件，不是解
			if (posiblePlug.size() == 0)
				return vector<Enigma>(0);

			if (posiblePlug.size() > 1) {
				// 解决多个解的死循环问题？
				if (repeatTimes == testPlugQueue.size()) {
					vector<Enigma> enigmaVec;
					for (int j : posiblePlug) {
						enigma_f.addPlugIn(testPlugChar, to_char(j));
						vector<Enigma> ansRecrusive = TestPlugIn(
							rings,
							enigma_f,
							enigma_NoPlug,
							testPlugQueue,
							ringsInfo
						);
						enigmaVec.insert(enigmaVec.end(), ansRecrusive.begin(), ansRecrusive.end());
						enigma_f.removePlugIn(testPlugChar, to_char(j));
					}
					return enigmaVec;
				}
				else {
					++repeatTimes;
					testPlugQueue.push(testPlugInt);
				}
				continue;
			}

			// 只有一个字符，添加plug
			for (int j : posiblePlug) {
				enigma_f.addPlugIn(testPlugChar, to_char(j));
				enigma_f.setPlugInUnchangable(testPlugChar);
				enigma_f.setPlugInUnchangable(to_char(j));
			}
			repeatTimes = 0;
		}
		vector<Enigma> enigmaVec;
		enigmaVec.push_back(enigma_f);
		return enigmaVec;
	}

	static vector<vector<pair<int, int>>> FindRingStart(const vector<vector<V>>& rings) {
		vector<vector<pair<int, int>>> ans(26);
		for (int i = 0; i < rings.size(); ++i)
			for (int j = 0; j < rings[i].size(); ++j)
				ans[rings[i][j].letter].push_back(pair<int, int>(i, j));
		return ans;
	}

public:
	bool PrintFlag_Encryption = false;

	Enigma() {
		char rotor_init_position[3] = { 'H', 'Y', 'Z' };
		int rotorSetting[3] = { 3, 0, 1 };
		int plugIn[26];
		for (int i = 0; i < 26; ++i) {
			plugIn[i] = i;
		}
		init(rotor_init_position, rotorSetting, plugIn);
	}
	Enigma(char rotor_init_position[3], int rotorSetting[3]) {
		int plugIn[26];
		for (int i = 0; i < 26; ++i) {
			plugIn[i] = i;
		}
		init(rotor_init_position, rotorSetting, plugIn);
	}
	Enigma(char rotor_init_position[3], int rotorSetting[3], int plugIn[26]) {
		init(rotor_init_position, rotorSetting, plugIn);
	}

	inline void setPlugInUnchangable(char a) {
		_plugInChangable[a - 'A'] = false;
	}

	inline void clearPlugIn() {
		for (int i = 0; i < 26; ++i) {
			_plugIn[i] = i;
		}
	}

	inline void resetRotorPosition() {
		for (int i = 0; i < 3; ++i) {
			_rotorLetter[i] = _rotorInitPosition[i];
		}
	}

	inline void setRotorPosition(char rotor_position[3]) {
		for (int i = 0; i < 3; ++i)
			_rotorLetter[i] = rotor_position[i] - 'A';
	}

	void PrintKey() const {
		printf("Rotor init position: %c%c%c\nRotor setting: (%d, %d, %d)\n",
			to_char(_rotorInitPosition[0]),
			to_char(_rotorInitPosition[1]),
			to_char(_rotorInitPosition[2]),
			_rotorSetting[0],
			_rotorSetting[1],
			_rotorSetting[2]
		);
		printf("Plug: ");
		for (int i = 0; i < 26; ++i)
			printf("%c->%c ", char(i + 'A'), char(_plugIn[i] + 'A'));
		printf("\n");
		printf("Plug: ");
		for (int i = 0; i < 26; ++i)
			printf("%d, ", _plugIn[i]);
		printf("\n\n");
	}

	char Encryption(char plainText) {
		rotateTimes(1);

		int plainInt = plainText - 'A';
		int letter = _plugIn[plainInt];

		if (PrintFlag_Encryption)
			printf("PlugIn Encryption : %c\n", to_char(letter));

		for (int i = 2; i >= 0; --i) {
			letter = _rotor[i][(letter + _rotorLetter[i]) % 26];
			letter = (letter - _rotorLetter[i] + 26) % 26;

			if (PrintFlag_Encryption)
				printf("Rotor %d Encryption : %c\n", i, to_char(letter));
		}

		letter = _reflector[letter];

		if (PrintFlag_Encryption)
			printf("Reflector Encryption : %c\n", to_char(letter));

		for (int i = 0; i < 3; ++i) {
			letter = (letter + _rotorLetter[i]) % 26;
			letter = (find(letter, _rotor[i]) - _rotorLetter[i] + 26) % 26;

			if (PrintFlag_Encryption)
				printf("Rotor %d Encryption : %c\n", i, to_char(letter));
		}


		letter = _plugIn[letter];

		if (PrintFlag_Encryption)
			printf("Encryption Result : %c\n", to_char(letter));

		return to_char(letter);
	}

	std::string Encryption(std::string plainText) {
		std::string cipherText(plainText.size(), '-');
		for (int i = 0; i < plainText.size(); ++i)
			cipherText[i] = Encryption(plainText[i]);
		return cipherText;
	}

	inline void rotateTimes(int times) {
		for (int i = 0; i < times; ++i)
			rotorRotate(2);
	}

	bool addPlugIn(char ch1, char ch2) {
		int n1 = ch1 - 'A', n2 = ch2 - 'A';
		if (_plugIn[n1] == n2 && _plugIn[n2] == n1)
			return true;
		if (!(_plugInChangable[n1] && _plugInChangable[n2]))
			return false;
		if (_plugIn[n1] != n1 || _plugIn[n2] != n2)
			return false;
		_plugIn[n1] = n2;
		_plugIn[n2] = n1;
		return true;
	}

	bool removePlugIn(char ch1, char ch2) {
		int n1 = ch1 - 'A', n2 = ch2 - 'A';
		if (!(_plugInChangable[n1] && _plugInChangable[n2]))
			return false;
		if (_plugIn[n1] == n1 && _plugIn[n2] == n2)
			return true;
		if (_plugIn[n1] == n2 && _plugIn[n2] == n1) {
			_plugIn[n1] = n1;
			_plugIn[n2] = n2;
			return true;
		}
		return false;
	}

	static void Crack(const string& plainText, const string& cipherText) {
		// 开始破译
		char rotor_init_position[3];
		int rotorSetting[3];

		// 首先找到所有环
		vector<vector<V>> rings = V::findRings(plainText, cipherText);
		vector<vector<pair<int, int>>> ringsInfo = FindRingStart(rings);
		pair<int, int> mostRingsLetter[26];
		for (int i = 0; i < 26; ++i)
			mostRingsLetter[i] = pair<int, int>(i, (int)ringsInfo[i].size());
		sort(mostRingsLetter, mostRingsLetter + 26,
			[](pair<int, int> x, pair<int, int> y) {
				return x.second > y.second;
			}
		);

		// 验证每个都是环
		for (int i = 0; i < rings.size(); ++i) {
			for (int j = 0; j < rings[i].size(); ++j) {
				assert(plainText[rings[i][j].index] - 'A' == rings[i][j].letter);
				assert(cipherText[rings[i][j].index] - 'A' == rings[i][(j + 1) % rings[i].size()].letter);
			}
		}
		// 打印下来
		for (int i = 0; i < rings.size(); ++i) {
			printf("Ring %d, length = %d : ", i + 1, (int)rings[i].size());
			for (int j = 0; j < rings[i].size(); ++j) {
				rings[i][j].print();
				printf("->");
			}
			rings[i][0].print();
			printf("\n");
		}
		printf("\n");


		// 打印所有符合条件的 enigma settings
		printf("Cracked all enigma settings: \n");

		vector<Enigma> enigma;
		// 从5个rotor中找3个，A53
		for (rotorSetting[0] = 0; rotorSetting[0] < 5; ++rotorSetting[0])
			for (rotorSetting[1] = 0; rotorSetting[1] < 5; ++rotorSetting[1]) {
				if (rotorSetting[0] == rotorSetting[1])
					continue;
				for (rotorSetting[2] = 0; rotorSetting[2] < 5; ++rotorSetting[2]) {
					if (rotorSetting[0] == rotorSetting[2] || rotorSetting[1] == rotorSetting[2])
						continue;
					// 遍历三个转子的初始位置
					for (rotor_init_position[0] = 'A'; rotor_init_position[0] <= 'Z'; ++rotor_init_position[0])
						for (rotor_init_position[1] = 'A'; rotor_init_position[1] <= 'Z'; ++rotor_init_position[1])
							for (rotor_init_position[2] = 'A'; rotor_init_position[2] <= 'Z'; ++rotor_init_position[2]) {
								bool succeed = true;

								// 用猜测的rotor和initial position来初始化enigma
								Enigma enigma_f(rotor_init_position, rotorSetting);
								// 这个enigma不加插板，专门用来跑ring
								Enigma enigma_NoPlug(rotor_init_position, rotorSetting);

								queue<int> testPlugQueue;
								// 按照环越多的字母优先考虑的原则
								for (int i = 0; i < 26; ++i)
									testPlugQueue.push(mostRingsLetter[i].first);

								vector<Enigma> enigmaVec = TestPlugIn(
									rings,
									enigma_f,
									enigma_NoPlug,
									testPlugQueue,
									ringsInfo
								);

								// 全insert到一起，速度慢一些
								enigma.insert(enigma.end(), enigmaVec.begin(), enigmaVec.end());
								/*for (const Enigma& i : enigmaVec)
									i.PrintKey();*/
							}
				}
			}

		printf("Num of Settings meeting the requirements = %d\n\n", (int)enigma.size());
		for (const Enigma& i : enigma)
			i.PrintKey();
	}
};


#endif