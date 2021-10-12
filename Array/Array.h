#pragma once
#include <stdlib.h>
#include <iostream>

using namespace std;

class Array {
private:
	const int N; //universe size
	int n; //set size
	char S, * Arr; //name, array
	bool exist(char s) const { //check, is this element in set
		bool b = false;
		for (int i = 0; Arr[i] && !b; ++i)
			if (Arr[i] == s) b = true;
		return b;
	}
public:
	Array(char s) : N(16), n(n), S(s) { //constructor
		n = rand() % (N + 1);
		Arr = new char[n + 1];
		GenerationSet(Arr, n);
	}

	Array() : N(16), n(0), S('E') {}; //simple constructor

	Array& operator & (const Array& B) const { //logic AND
		Array* E = new Array();
		int c = 0;
		for (int i = 0; Arr[i]; ++i)
			if (B.exist(Arr[i])) c++;
		E->Arr = new char[c + 1];
		for (int i = 0; Arr[i]; ++i)
			if (B.exist(Arr[i])) E->Arr[E->n++] = Arr[i];
		return *E;
	}

	Array& operator | (const Array& B) { //logic OR
		Array* E = new Array();
		int k = 0;
		for (int i = 0; Arr[i]; ++i) k++;
		for (int i = 0; B.Arr[i]; ++i)
			if (!(this->exist(B.Arr[i]))) k++;
		cout << k << endl;
		E->Arr = new char[k + 1];

		for (int i = 0; Arr[i]; ++i)
			E->Arr[E->n++] = Arr[i];

		for (int i = 0; B.Arr[i]; ++i)
			if (!(this->exist(B.Arr[i])))
				E->Arr[E->n++] = B.Arr[i];

		return *E;
	}
	/*Array& operator |= (const Array&); // объединение и присваивание
	Array& operator &= (const Array&); // пересечение и присваивание*/

	Array& operator ~ () const { //logic NOT
		char U[] = "0123456789ABCDEF";
		Array* E = new Array();
		E->Arr = new char[N - n + 1];
		for (int i = 0; U[i]; ++i)
			if (!(this->exist(U[i]))) E->Arr[E->n++] = U[i];
		E->Arr[E->n] = 0;
		return *E;
	}
	void Show() { //print set
		cout << S << " = ";
		if (n > 0)
			for (int i = 0; i < n; i++) cout << Arr[i] << " ";
		else cout << "empty";
		cout << endl;
	}
	int power() const { return n; } //to see private size of set

	Array& operator = (const Array& A) { //assignment
		n = A.n;
		Arr = new char[n + 1];
		for (int i = 0; i < A.n; ++i) Arr[i] = A.Arr[i];
		Arr[n] = '\0';
		return *this;
	}
	~Array() { //destructor (doesn't work??)
		delete Arr;
	}

	/*char GenerationSet() {
		char U[] = "0123456789ABCDEF";
		for (int i = 0; i < n; i++) {
			int m = rand() % (n - i);
			Arr[i] = U[i + m];
			U[i + m] = U[i];
		}
		Arr[n] = 0;
		return Arr;
	} //??*/

	void GenerationSet(char* Arr, int n) {
		char U[] = "0123456789ABCDEF";
		int i = 0;
		Arr[i++] = U[rand() % (15 - 0)]; //initialize first element of array
		while (i < n) {
			int flag = 0; //to find repeats
			char a = U[rand() % (15 - 0)];
			for (int j = 0; flag == 0 && j <= i; ++j)
				if (Arr[j] == a)  flag = 1;
			if (flag == 0)
				Arr[i++] = a;
		}
		Arr[n] = 0;
	}
};
