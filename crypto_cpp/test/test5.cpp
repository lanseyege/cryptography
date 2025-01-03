#include <iostream>
#include "../bigint.h"

using namespace std;

int main () {
    BigInt bigint;
    int len = 120;
    string prime = bigint.generate_prime(len);
    cout <<"generated prime: " << prime << endl;
}
