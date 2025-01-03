#include <pbc/pbc.h>
#include <gmpxx.h>
#include <iostream>
#include <cryptopp/sha.h>
#include <cryptopp/sm3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using namespace std;
using namespace CryptoPP;

/**
void get_random_element() {
    
    mpz_urandomm( , _gstate, );
}
**/
int main()  {

    /**
    clock_t _time;
    gmp_randstate_t _gstate;
    mpz_t _temp;
    _time = clock();
    gmp_randint_default(_gstate);
    gmp_randseed_ui(_gstate, _time);
    **/

    SHA256 sha256;
    pairing_t pairing;
    char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    cout << "count : " << count << endl;
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_t P, h;
    element_t s;
    element_t public_key, secret_key; 
    
    element_init_G1(P, pairing);
    element_init_Zr(secret_key, pairing);

    element_random(P);
    element_random(secret_key);
    
    element_mul_zn(public_key, secret_key, P);

}
