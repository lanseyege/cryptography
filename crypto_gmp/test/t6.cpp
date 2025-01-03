#include <iostream>

using namespace std;

int main() {
    int naf[10];
    naf[0] = 10;
    naf[1] = 2;
    naf[2] = '\0';
    cout << sizeof(naf) / sizeof(int) << endl;
}
