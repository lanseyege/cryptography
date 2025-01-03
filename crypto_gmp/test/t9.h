#include <iostream>
#include <stdio.h>
#include <gmpxx.h>
#include <sstream>

#define MAX_ARRAY_LEN 10000

using namespace std;

class Crypt {

    public:
        Crypt();
        ~Crypt();
        // EC
        void Ecc_Init(string init_keys);
        void ECC_Generate_Keys(string init_keys);
        void ECC_Encryption(string message, mpz_t C1_x, mpz_t C1_y, mpz_t C2_x, mpz_t C2_y);
        void ECC_Decryption(mpz_t C1_x, mpz_t C1_y, mpz_t C2_x, mpz_t C2_y);
        void ECC_Sign();
        int ECC_Verify_Sign();
    

        gmp_randstate_t _gstate;
        clock_t _time;

        int prime_test_reps;
        // EC
        mpz_t _ecc_p;
        mpz_t _ecc_a;
        mpz_t _ecc_b;
        mpz_t _ecc_Gx;
        mpz_t _ecc_Gy;
        mpz_t _ecc_n;
        mpz_t _ecc_h;
        mpz_t _ecc_d_key;
        mpz_t _ecc_Qx_key;
        mpz_t _ecc_Qy_key;
        mpz_t _ecc_embed_k;
        string _ecc_stand[2];

        int _ecc_point_addition(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y, mpz_t three_x, mpz_t three_y);
        int _ecc_point_doubling(mpz_t one_x, mpz_t one_y);
        int _ecc_point_doubling(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y);
        void _ecc_point_multiplication(mpz_t _K, mpz_t _ecc_t1, mpz_t _ecc_t2, mpz_t Q_x, mpz_t Q_y);
        int * _ecc_NAF(mpz_t _K);
        void _ecc_cyber_text_decode(mpz_t _mess_x, mpz_t _mess_y, mpz_t _message);
        void _ecc_plain_text_embeded(string message,  mpz_t _mess_x, mpz_t _mess_y);
        string _hex_to_string(string message);
        string _string_to_hex(string message);

};

