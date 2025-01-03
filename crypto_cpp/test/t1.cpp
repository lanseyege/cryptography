#include <bits/stdc++.h>

using namespace std;

void coprime(int a, int b) {
    if (__gcd(a, b) == 1) 
        cout << "Co-Prime" <<endl;
    else
        cout << "Not Co-Prime" <<endl;
}

int main() {
    long long a = 444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444445;
    long long b = 67777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777770011223445677777;
    coprime(a, b);
    a = 8, b = 16;
    coprime(a, b);
    return 0;
}

