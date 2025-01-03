#include <iostream>
#include <gmpxx.h>

using namespace std;

int main() {
    mpz_t t ; mpz_init(t);
    mpz_t t2 ; mpz_init(t2);
    int *_naf = new int[1000];
    //mpz_set_str(t, "6056313465948974399", 10);
    mpz_set_str(t, "38081497578468531440476294253657802530702533258463576127655285360072642712661", 10);
    gmp_printf("t : %Zd\n", t);
    int i = 1;
    while(mpz_cmp_ui(t, 1) >= 0)  {
        if (mpz_odd_p(t) != 0) {
            mpz_mod_ui(t2, t, 4);
            _naf[i] = 2 - (int) mpz_get_si(_temp);
            mpz_sub_ui();:q

        }
        mpz_fdiv_q_ui(t, t, 2);
        gmp_printf("t : %Zd\n", t);
    
    }  
    //mpz_cdiv_q_ui(t2, t, 2);
    //gmp_printf("t2 : %Zd\n", t2);
    //mpz_tdiv_q_ui(t2, t, 2);
    //gmp_printf("t2 : %Zd\n", t2);
    delete _naf;

}
