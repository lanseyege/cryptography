#include "ibe.h"


BasicIdent::BasicIdent() {
}
void BasicIdent::basic_ibe_setup() {
    
    _basic_ibe_k = 256;
    _time = clock();
    gmp_randint_default(_gstate);
    gmp_randseed_ui(_gstate, _time);

    mpz_urandom(_basic_ibe_q, _gstate, _basic_ibe_k);
}
