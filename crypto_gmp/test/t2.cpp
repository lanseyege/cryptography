#include <gmpxx.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
    /*
    mpz_t a, b, c;
    mpz_init(a);
    mpz_init(b);
    mpz_init(c);
    gmp_scanf("%Zd%Zd", a, b) ;
    mpz_add(c, a, b) ;
    gmp_printf("c =  %Zd\n", c) ; */
    //urandomb()
    //

    //unsigned long k = gmp_urandomb_ui();
    clock_t time = clock();
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);

    mpz_t p;
    mpz_init(p);

    mpz_urandomb(p, grt, 512*2);
    mpz_nextprime(p, p) ;

    mpz_t q;
    mpz_init(q);
    mpz_urandomb(q, grt, 512*2);
    mpz_nextprime(q, q) ;

    gmp_printf("prime p: %Zd\n", p);
    gmp_printf("prime q: %Zd\n", q);

    mpz_t m;
    mpz_init(m);
    mpz_mul(m, p, q);
    gmp_printf("multi res m: %Zd\n", m);

    mpz_clear(m) ;
    mpz_clear(p) ;
    mpz_clear(q) ;

    return 0;
}
