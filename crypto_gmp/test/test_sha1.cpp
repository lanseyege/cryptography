#include "../hash_algorithms.h"

int main() {

    HashAlgorithms hash;
    hash.sha1_init();
    string message = "The quick brown fox jumps over the lazy dog";
    message = "abc";   
    string value = hash.sha1_value(message);
    cout << value << endl;
}
