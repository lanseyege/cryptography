#include <iostream>

#include "../bigint.h"

int main() {
    HashAlgorithms hash ;
    string message = "hello" ;
    message = "The quick brown fox jumps over the lazy dog.";
    //message = "";
    hash.MD5_init();
    string res = hash.MD5_value(message);
    cout << "res: " << res << endl;
}
