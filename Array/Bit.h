#pragma once
#include <iostream>

using namespace std;

class Set {
private:
	const int N; //universe size
	int n; //set size
	char S;	//name
	bool Arr[16]; //array
public:
	Set(char s) : N(16), n(0), S(s) { //constructor
		GenerationSet();
	}

	Set() : N(16), n(0), S('E') {}; //simple constructor

	Set operator & (const Set& B) const { //logic AND
		Set E;
		for (int i = 0; i < N; i++)
		{
			if (Arr[i] && B.Arr[i])
			{
				E.Arr[i] = true;
				E.n++;
			}
		}
		return E;
	}

	void Show() { //print set
		const char U[] = "0123456789ABCDEF";
		cout << S << "(" << n << ") = ";
		if (n != 0)
			for (int i = 0; i < N; i++) {
				if (Arr[i] == 1) {
					cout << U[i] << ' ';
				}
			}
		else cout << "empty";
		cout << endl;
	}
	int power() const { return n; } //to see private size of set

	Set& operator = (const Set& A) { //assignment
		for (int i = 0; i < N; ++i) {
			Arr[i] = A.Arr[i];
		}
		n = A.n;
		return *this;
	}

	void GenerationSet() {
		for (int i = 0; i < N; i++)
		{
			Arr[i] = (rand() & 1);
			if (Arr[i]) {
				n++;
			}
		}
	}
};
