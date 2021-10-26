#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class El {
private:
	char d;
	El* next;
	El() : d('!'), next(nullptr) { }
	El(char d, El* p = nullptr) : d(d), next(p) { }
	~El() { delete next; }
public:
	static const int maxmup = 300;
	static El mem[maxmup];
	static int mup, mup0;
	static void* operator new(size_t) {
		return (mup < maxmup ? &mem[mup++] : nullptr);
	}
	static void operator delete(void*, size_t) { }

	friend static void memOut();
	friend class Set;
};
void memOut() {
	std::cout << "\nAll memory for list - " << El::mup << endl;
	for (int i = 0; i < El::mup; ++i) cout << El::mem[i].d << '-';
	cout << endl;
}

El El::mem[El::maxmup];
int El::mup = 0, El::mup0 = 0;

class Set {
	const int N;
	int n;
	static int num;
	char S;
	El* A;
public:
	Set(char s) : N(16), n(n), S('A' + num++), A(nullptr) { GenerationSet(); cout << "Create " << S << ": "; Show(); }
	Set() : N(16), n(0), S('A' + num++), A(nullptr) { cout << "Create " << S << ": "; Show(); };
	Set(const Set& B) : N(16), n(B.n), S('A' + num++), A(nullptr) {
		for (El* i = B.A; i; i = i->next) A = new El(i->d, A);
		cout << "Copy " << B.S << " to " << S << ": "; Show();
	}
	~Set() { cout << "Delete " << S << ": "; Show(); delete A; }

	Set& operator = (const Set& B) {
		n = B.n;
		delete A; A = nullptr;
		for (El* i = B.A; i; i = i->next) A = new El(i->d, A);
		return *this;
	}

	Set& operator = (Set&& B) noexcept {
		n = B.n; A = B.A; B.A = nullptr; B.n = 0;
		cout << "Assigment " << B.S << " to " << S << ": "; Show();
		return *this;
	}

	Set operator & (const Set& B) const {
		Set E;
		for (El* i = A; i; i = i->next) if (B.Existence(i->d)) {
			E.A = new El(i->d, E.A);
			E.n++;
		}
		return E;
	}

	void GenerationSet();
	void Show();
	bool Existence(char s) const;
};

int Set::num = 0;

void Set::GenerationSet() {
	n = rand() % (N + 1);
	char U[] = "0123456789ABCDEF";
	for (int i = 0; i < n; i++) {
		int m = rand() % (N - i);
		A = new El(U[i + m], A);
		U[i + m] = U[i];
	}
}

void Set::Show() {
	std::cout << S << "(" << n << ") = ";
	if (n > 0) for (El* i = A; i; i = i->next) std::cout << (*i).d << ' ';
	else std::cout << "empty";
	std::cout << std::endl;
}

bool Set::Existence(char s) const {
	El* i = A;
	while (i && i->d != s) i = i->next;
	return i;
}

int main()
{
	srand(time(nullptr));

	Set A('A'), B('B'), C('C'), D('D'), R;
	R = A & B & C & D;

	return 0;
}
