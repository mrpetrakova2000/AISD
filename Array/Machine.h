#pragma once
#include <iostream>

using namespace std;

class Set {
private:
	const int N; //universe size
	int n; //set size
	char S;	//name
	unsigned short Arr; 
public:
	Set(char s) : N(16), n(0), S(s) { //constructor
		GenerationSet();
	}

	Set() : N(16), n(0), S('E') {}; //simple constructor

	Set operator & (const Set& B) const { //logic AND
		Set E;
		E.Arr = Arr & B.Arr;
		return E;
	}

	void Show() { //print set
		const char U[] = "0123456789ABCDEF";
		for (int i = 0; i < 16; i++) { //n - search
			if ((Arr & (1 << i)) >> i) {
				n++;
			}
		}
		cout << S << "(" << n << ") = ";
		for (int i = 0; i < 16; i++) {
			if ((Arr & (1 << i)) >> i) {
				cout << U[i] << ' ';
			}
		}
		if (n == 0) cout << "empty";
		cout << endl;
	}
	int power() const { return n; } //to see private size of set

	Set& operator = (const Set& A) { //assignment
		for (int i = 0; i < N; ++i) {
			Arr = A.Arr;
		}
		n = A.n;
		return *this;
	}

	void GenerationSet() {
		Arr = rand() % 65536;
	}
};
