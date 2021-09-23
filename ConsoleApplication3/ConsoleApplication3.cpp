#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
    using namespace std;

    clock_t start;
    start = clock();
    srand(time(nullptr));

    const char U[] = "0123456789ABCDEF";

    unsigned short Sets[4];
    for (int i = 0; i < 4; i++) Sets[i] = rand() % 65535;
    
    for (int i = 0; i < 4; i++) {
        cout << char('A' + i) << ": ";
        for (int j = 0; j < 16; j++) if ((Sets[i] & 1 << j) >> j) cout << U[j] << ' ';
        cout << endl;
    }
    
    unsigned short R = Sets[0];
    for (int i = 1; i < 4; i++) R &= Sets[i];

    cout << "E: ";
    for (int i = 0; i < 16; i++) if ((R & (1 << i)) >> i) cout << U[i] << ' ';
    cout << endl;

    cout << "Duration: " << (clock() - start) / (double)CLOCKS_PER_SEC << endl;

    return 0;
}

