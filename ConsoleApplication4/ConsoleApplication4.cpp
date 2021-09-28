#include <iostream>
#include <random>

using namespace std;

struct EL
{
	char d;
	EL* next;
	EL(char d, EL* next) : d(d), next(next) { }
};


int main()
{
	srand(time(nullptr));

	clock_t start;
	start = clock();


	string digits = "0123456789ABCDF";
	EL* nums = nullptr;
	EL* La = nullptr, * Lb = nullptr, * Lc = nullptr, * Ld = nullptr, * Le = nullptr;
	EL* tmp;
	for (int i = 0; i < 16; i++)
		if (rand() % 2)
			La = new EL(digits[i], La);
	for (int i = 0; i < 16; i++)
		if (rand() % 2)
			Lb = new EL(digits[i], Lb);
	for (int i = 0; i < 16; i++)
		if (rand() % 2)
			Lc = new EL(digits[i], Lc);
	for (int i = 0; i < 16; i++)
		if (rand() % 2)
			Ld = new EL(digits[i], Ld);

	cout << "A =";
	for (EL* v = La; v; v = v->next)
	{
		cout << ' ' << v->d;
	}
	cout << endl << "B =";
	for (EL* v = Lb; v; v = v->next)
	{
		cout << ' ' << v->d;
	}
	cout << endl << "C =";
	for (EL* v = Lc; v; v = v->next)
	{
		cout << ' ' << v->d;
	}
	cout << endl << "D =";
	for (EL* v = Ld; v; v = v->next)
	{
		cout << ' ' << v->d;
	}
	cout << endl << endl << "A & B & C & D =";

	for (EL* B = Lb; B; B = B->next)
		for (EL* C = Lc; C; C = C->next)
			for (EL* D = Ld; D; D = D->next)
				for (EL* A = La; A; A = A->next)
					if (B->d == C->d && C->d == D->d && A->d == B->d)
					{
						int flag = true;
						for (EL* num = nums; num; num = num->next)
							if (D->d == num->d)
							{
								flag = false;
								break;
							}
						if (flag)
						{
							nums = new EL(D->d, nums);
							Le = new EL(D->d, Le);
						}
					}

	for (EL* v = Le; v; v = v->next)
	{
		cout << ' ' << v->d;
	}
	cout << endl;
	cout << "Duration: " << (clock() - start) / (double)CLOCKS_PER_SEC << endl;
	return 0;
}