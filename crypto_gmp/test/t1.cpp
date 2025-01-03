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

    mpz_t key;
    mpz_init(key);

    mpz_urandomb(key, grt, 512);
    gmp_printf("%Zd\n", key);

    int res = mpz_probab_prime_p(key, 24);
    if (res == 0) cout << "definitely no - prime" << endl;
    else if(res == 2) cout << "definitely prime" << endl;
    else cout << "probability prime " << endl;

    int i = 0; 
    while(res == 0) {
        mpz_urandomb(key, grt, 512);
        gmp_printf("%Zd\n", key);

        res = mpz_probab_prime_p(key, 24);
        i ++;
    }
    cout << "i: " << i << endl;
    mpz_clear(key) ;
    return 0;
}
