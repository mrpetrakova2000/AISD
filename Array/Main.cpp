#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Array.h"

using namespace std;
int main()
{
    srand(time(nullptr));
    Array A('A'), B('B'), C('C'), D('D');
    Array E, F, G;
    A.Show();
    B.Show();
    C.Show();
    D.Show();

    F = A & B;
    G = F & C;
    E = G & D;
    cout << endl;
    cout << "A & B & C & D:\n";
    E.Show();

    ~A; ~B; ~C; ~D; ~E;
    return 0;
}