#ifndef V_H_
#define V_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

// �ַ���index�Ľṹ
struct V {
	int index;
	int letter;

	bool operator==(const V& other) const {
		return letter == other.letter && index == other.index;
	}

	bool operator<(const V& other) const {
		return index < other.index;
	}

	void print() const {
		printf("(%c, %d)", char(letter + 'A'), index);
	}

	// �ҵ������Ķ��������Ȧ���������ظ�Ԫ�أ�
	static vector<vector<V>> findRings(const string& plainText, const string& cipherText) {
		vector<vector<V>> ans;
		map<V, vector<V>> separate = separateLetters(plainText, cipherText);
		vector<V> ring;
		set<V> added;
		set<V> visited;
		for (const auto& j : separate) {
			if (added.find(j.first) != added.end())
				continue;
			ring.clear();
			visited.clear();
			ring.push_back(j.first);
			visited.insert(j.first);
			findRings(separate, ans, visited, ring, added);
		}
		return ans;
	}

private:
	// �ݹ���Ȧ
	static void findRings(map<V, vector<V>> separate, vector<vector<V>>& ans, set<V>& visited, vector<V>& ring, set<V>& added) {
		V lastPoint = ring.back();
		for (const V& i : separate[lastPoint]) {
			if (added.find(i) != added.end())
				continue;

			if (visited.find(i) == visited.end()) {
				// ��û�л����ݹ���Ȧ
				visited.insert(i);
				ring.push_back(i);
				findRings(separate, ans, visited, ring, added);
				visited.erase(i);
				ring.pop_back();
			}
			else {
				// �Ѿ��л��ˣ��������� A->B->C->B
				// �ҳ������Ļ����� B->C->B�������� A
				for (int index = 0; index < ring.size(); ++index)
					if (ring[index] == i) {
						vector<V> realRing;
						for (int j = index; j < ring.size(); ++j) {
							realRing.push_back(ring[j]);
							added.insert(ring[j]);
						}
						ans.push_back(realRing);
						break;
					}
			}
		}
	}

	// ����������ÿ��λ�õ�key���� index=2 �� A->B��������һ��������B��λ�õ�ӳ��
	static map<V, vector<V>> separateLetters(const string& plainText, const string& cipherText) {
		map<V, vector<V>> ans;
		vector<V> cipherV[26];
		for (int i = 0; i < plainText.size(); ++i) {
			V v = { i, plainText[i] - 'A' };
			cipherV[v.letter].push_back(v);
		}
		for (int i = 0; i < plainText.size(); ++i) {
			V v = { i, plainText[i] - 'A' };
			ans[v] = cipherV[cipherText[i] - 'A'];
		}
		return ans;
	}
};

#endif