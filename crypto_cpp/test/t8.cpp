#include <iostream>

using namespace std;

int main() {
    string s = "hello";
    int num = 1, i;
    int * byt = new int[num * 16] ;
    for (i =0; i < s.length(); i++) {
        cout << byt[i >> 2] << " ." << endl;
        cout << s.at(i) << " .." << (int) (s.at(i)) << endl;
        cout << (s.at(i) << ((i % 4) * 8)) << " -" << endl;;
        byt[i >> 2] |= s.at(i) << ((i % 4) * 8);
        cout << byt[i >> 2] << " =" << endl;

    }
    byt[i >> 2] |= 0x80 << ((i % 4) * 8) ;
    for (i = 0; i < num * 16; i++)
        cout << byt[i] <<" " << endl;
    //cout << (5 >> 2) << endl;
}
