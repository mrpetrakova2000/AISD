#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Array.h"

using namespace std;
int main()
{
    //cout << "all ok\n";
    srand(time(nullptr));
    Array A('A'), B('B'), C('C'), D('D');
    Array E;
    A.Show();
    B.Show();
    C.Show();
    D.Show();

    E = A & B & C & D;
    cout << endl;
    cout << "A & B & C & D:\n";
    E.Show();

    return 0;
}