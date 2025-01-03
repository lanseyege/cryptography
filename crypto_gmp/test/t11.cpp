#include <gmpxx.h>
#include <iostream>

using namespace std;

int main() {
    mpz_t t; mpz_init(t);
    mpz_t t2; mpz_init(t2);
    mpz_t t3; mpz_init(t3);
    
    mpz_set_si(t, 0);
    mpz_set_si(t3, 23);
    gmp_printf("t3: %Zd\n", t3);
    mpz_invert(t2, t, t3);
    gmp_printf("t2: %Zd\n", t2);
    mpz_mul(t2, t, t3);
    //mpz_mul_ui(t3, t3, -1);
    gmp_printf("t3: %Zd\n", t3);
    mpz_abs(t3, t3);
    gmp_printf("t2: %Zd\n", t2);
    gmp_printf("t3: %Zd\n", t3);

}
