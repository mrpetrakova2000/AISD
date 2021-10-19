#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Array.h"

using namespace std;
int main()
{
    srand(time(nullptr));
    Set A('A'), B('B'), C('C'), D('D');
    Set E;
    A.Show();
    B.Show();
    C.Show();
    D.Show();

    E = A & B & C & D;
    cout << endl;
    cout << "A & B & C & D:\n";
    return 0;
}