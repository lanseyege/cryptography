#include <gmpxx.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void fun(mpz_t a, mpz_t b, mpz_t c) {
    
    mpz_add(c, a, b);

    return;
}

int main() {
    mpz_t rop, op1, op2; mpz_inits(rop, op1, op2);
    mpz_set_str(op1, "3", 10); mpz_set_str(op2 , "19", 10);
    mpz_invert(rop, op1, op2) ;
    gmp_printf("%Zd\n", rop);
    mpz_t c; mpz_init(c);
    fun(op1, op2, c);

    gmp_printf("%Zd\n" , c);

    mpz_t prime; mpz_init(prime);

    gmp_randstate_t _gstate;
    clock_t _time;
    _time = clock();
    gmp_randinit_default(_gstate);
    gmp_randseed_ui(_gstate, _time);
    mpz_urandomb(prime, _gstate, 100) ;
    gmp_printf("%Zd\n", prime);
    mpz_nextprime(prime, prime) ;
    gmp_printf("%Zd\n", prime);

    mpz_urandomb(prime, _gstate, 100) ;
    gmp_printf("%Zd\n", prime);
    mpz_nextprime(prime, prime) ;
    gmp_printf("%Zd\n", prime);


}
