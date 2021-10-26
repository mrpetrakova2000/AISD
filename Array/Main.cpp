#include <iostream>
#include <stdlib.h>
#include <time.h>
//#include "Array.h"
//#include "Bit.h"
//#include "Machine.h"
//#include "List.h"
//#include "ExperienceWithOverloadList.h"

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

    
    clock_t t = clock();
    for (int i = 0; i < 1000000; i++) Set R = A & B & C & D;
    t = clock() - t;
    cout << "Time: " << t / (double)CLOCKS_PER_SEC << endl;
    

    E = A & B & C & D;
    cout << endl;
    cout << "A & B & C & D:\n";
    E.Show();

    return 0;
}