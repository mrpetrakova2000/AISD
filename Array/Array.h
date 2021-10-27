#pragma once
#include <stdlib.h>
#include <iostream>

using namespace std;

class Set {
private:
	const int N; 
	int n; 
	char S, * Arr; 
	bool exist(char s) const {
		bool b = false;
		for (int i = 0; Arr[i] && !b; ++i)
			if (Arr[i] == s) b = true;
		return b;
	}
public:
	Set(char s) : N(16), n(0), S(s), Arr(nullptr) {
		GenerationSet();
		//GenerationSetN(10);
	}

	Set() : N(16), n(0), S('E'), Arr(nullptr) {};

	Set(const Set& B) : N(16), n(B.n), S('E') {
		Arr = new char[n + 1];
		for (int i = 0; B.Arr[i]; ++i) Arr[i] = B.Arr[i];
		Arr[n] = '\0';
	}

	~Set() { delete[] Arr; }
	
	Set& operator = (const Set& A) { 
		n = A.n;
		delete Arr; Arr = nullptr;
		Arr = new char[n + 1];
		for (int i = 0; i < n; ++i) Arr[i] = A.Arr[i]; Arr[n] = '\0';
		return *this;
	}

	Set operator & (const Set& B) const { 
		Set E;
		int c = 0;
		//контрольный вывод промежуточных результатов
		/*cout << endl;
		for (int i = 0; i < n; ++i) cout << Arr[i] << " ";
		cout << endl;
		for (int i = 0; i < B.n; ++i) cout << B.Arr[i] << " ";
		cout << endl;*/

		for (int i = 0; i < n; ++i)
			if (B.exist(Arr[i])) c++;
		E.Arr = new char[c + 1];
		for (int i = 0; i < n; ++i)
			if (B.exist(Arr[i])) 
				E.Arr[E.n++] = Arr[i];
		E.Arr[E.n] = '\0';
		return E;
	}

	void Show() {
		cout << S << "(" << n << ") = ";
		if (n > 0)
			for (int i = 0; i < n; i++) cout << Arr[i] << " ";
		else cout << "empty";
		cout << endl;
	}

	void GenerationSet() {
		char U[] = "0123456789ABCDEF";
		long w = 0;
		w = rand() % 65536;
		
		for (int i = 0; i < N; ++i)
			if ((w >> i) & 1) n++;
		Arr = new char[n + 1];

		for (int i = 0, k = 0; i < N; ++i)
			if ((w >> i) & 1) Arr[k++] = U[i];
		Arr[n] = '\0';
	}

	void GenerationSetN(int k) {
		char X[] = "0123456789ABCDEF";
		for (int i = 0; i < k; ++i) {
			int p = rand() % (N - i);
			if (p) swap(X[i], X[i + p]);
		}
		X[k] = '\0';
		Arr = new char[k + 1];
		for (int i = 0; i < k; ++i) {
			Arr[i] = X[i];
			n++;
		}
		Arr[n] = '\0';
	}
};
