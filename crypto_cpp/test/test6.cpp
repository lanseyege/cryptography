#include <iostream>
#include "../bigint.h"

using namespace std;


int main() {
    RSA rsa;
    int p_length = 150, q_length = 130;
    // generate public and private keys
    rsa.RSA_Plain_Generate_Keys(p_length, q_length);
    string message = "2223334445555566666666777777788888889999999"; 
    cout << "message encryption is: " << message << endl;
    // encryption 
    //string C = bigint.RSA_Plain_Encryption(message);
    string signature = rsa.RSA_Plain_Sign(message);
    //cout << "message encrypted  is: " << C << endl; 
    cout << " signature: " << signature << endl;
    // decryption  
    //string M = bigint.RSA_Plain_Decryption(C);
    //cout << "message decryted   is: " << M << endl;
    int acc = rsa.RSA_Plain_Verify_Sign(message, signature);
    cout << " acc: " << acc << endl;
}
