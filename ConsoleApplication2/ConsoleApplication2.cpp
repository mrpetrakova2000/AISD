#include <iostream>
#include <stdlib.h>
#include <time.h>

const char U[] = "0123456789ABCDEF";

bool* GenerationSet() {
    bool* A = new bool[16]{};
    for (int i = 0; i < 16; i++) if (rand() % 2) A[i] = 1;
    return A;
}

int main()
{
    using namespace std;

    srand(time(nullptr));

    bool** Sets = new bool* [4];
    for (int i = 0; i < 4; i++) Sets[i] = GenerationSet();

    for (int i = 0; i < 4; i++) {
        cout << char('A' + i) << ": ";
        for (int j = 0; j < 16; j++) if (Sets[i][j]) cout << U[j] << ' ';
        cout << endl;
    }

    bool* E = new bool[16];
    for (int i = 0; i < 16; i++) E[i] = Sets[0][i] && Sets[1][i] && Sets[2][i] && Sets[3][i];
    cout << "E: ";
    for (int i = 0; i < 16; i++) if (E[i]) cout << U[i] << ' ';
    cout << endl;

    clock_t start;
    start = clock();
    for (int t = 0; t < 1000000; t++) for (int i = 0; i < 16; i++) E[i] = Sets[0][i] && Sets[1][i] && Sets[2][i] && Sets[3][i];
    start = clock() - start;
    cout << "Duration: " << start / (double)CLOCKS_PER_SEC << endl;


    for (int i = 0; i < 4; i++) delete[] Sets[i];
    delete[] Sets;
    delete[] E;

    return 0;
}
