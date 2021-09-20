#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* GenerationSet() {
    int n = rand() % (16 + 1);
    char U[] = "0123456789ABCDEF";
    char* A = new char[n + 1];
    for (int i = 0; i < n; i++) {
        int m = rand() % (n - i);
        A[i] = U[i + m];
        U[i + m] = U[i];
    }
    A[n] = '\0';
    return A;
}

int main()
{
    using namespace std;

    clock_t start;
    start = clock();
    srand(time(nullptr));

    char** Sets = new char* [4];
    for (int i = 0; i < 4; i++) Sets[i] = GenerationSet();
    int lenght[] = { strlen(Sets[0]), strlen(Sets[1]), strlen(Sets[2]), strlen(Sets[3]) };

    for (int i = 0; i < 4; i++) {
        cout << char('A' + i) << ": ";
        for (int j = 0; j < lenght[i]; j++) cout << Sets[i][j] << ' ';
        cout << endl;
    }

    char* E = new char[16 + 1];
    int m = 0;
    for (int i1 = 0; i1 < lenght[0]; i1++) {
        bool flag = 1;
        for (int i2 = 0; i2 < lenght[1] && flag; i2++)
            if (Sets[0][i1] == Sets[1][i2])
                for (int i3 = 0; i3 < lenght[2] && flag; i3++)
                    if (Sets[1][i2] == Sets[2][i3])
                        for (int i4 = 0; i4 < lenght[3] && flag; i4++)
                            if (Sets[2][i3] == Sets[3][i4]) {
                                E[m] = Sets[3][i4];
                                m++;
                                flag = 0;
                            }
    }
    E[m] = '\0';
    cout << "E: ";
    for (int i = 0; i < m; i++) cout << E[i] << ' ';
    cout << endl;

    for (int i = 0; i < 4; i++) delete[] Sets[i];
    delete[] Sets;
    delete[] E;

    cout << "Duration: " << (clock() - start) / (double)CLOCKS_PER_SEC << endl;

    return 0;
}
