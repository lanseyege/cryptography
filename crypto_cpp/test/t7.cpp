#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <bitset>

using namespace std;


int main () {
    int len = 10;
    char *A = new char[len] ; 
    string message = "hello" ;
    A[0] = 'a'; 
    strcpy(A, message.c_str());
    for(int i = 5 ; i < len; i++ ) {
        //A[i] = 0x00;
        memset(A + i, 0x00, 1);
    }
    cout << sizeof(A) / sizeof(A[0]) << endl;
    cout << strlen(A) << endl;
    cout << "size of A: " << sizeof(A) << " size -: " << sizeof(A[0]) << endl;

    cout << sizeof(char) << endl;
    cout << sizeof(int) << endl;

    char T[9] = "abcdefgh";
    cout << "T : " << T << endl;
    int *B = new int[2] ;
    B[0] = T[3] + (T[2] << 8) + (T[1] << 16) + (T[0] << 24);
    B[1] = T[7] + (T[6] << 8) + (T[5] << 16) + (T[4] << 24);
    //memcpy(B, T, 4) ;
    //memcpy(B + 1, T + 4, 4);
    cout << bitset<32>(B[0]) << endl;
    for (int i = 0; i < 4; i++) 
        cout << bitset<8>(T[i]) << " " << T[i] << endl;
    cout << bitset<32>(B[1]) << endl;
    for (int i = 0; i < 4; i++)
        cout << bitset<8>(T[i+4]) << " " << T[i + 4]  << endl;
    cout << B << endl;
}
