#include <iostream>
#include <stdio.h>
#include <gmpxx.h>
#include <sstream>

#include "hash_algorithms.h"
#define MAX_ARRAY_LEN 10000

using namespace std;

class Crypt {

    public:
        Crypt();
        ~Crypt();
        // RSA
        void RSA_Plain_Generate_Keys(int p_length, int q_length, int is_out);
        void RSA_Plain_Encryption(const char* message, mpz_t _C);
        void RSA_Plain_Decryption(mpz_t _C, mpz_t _M) ;
        void RSA_Plain_Sign(const char* message, mpz_t signature, int is_out); 
        int RSA_Plain_Verify_Sign(const char* message, mpz_t signature, int is_out) ;
        // ElGamal
        void DL_Generate_Keys(int q_length, int is_out);
        void DL_ElGamal_Encryption(const char* message, mpz_t _C1, mpz_t _C2, int is_out);
        void DL_ElGamal_Decryption(mpz_t _C1, mpz_t _C2, mpz_t _M, int is_out);
        void DL_ElGamal_Sign(const char* message, mpz_t _R, mpz_t _S, int is_out);
        int DL_ElGamal_Verify_Sign(const char* message, mpz_t _R, mpz_t _S, int is_out);
        // EC
        void Ecc_Init(string init_keys);
        void ECC_Generate_Keys(string init_keys);
        void ECC_Encryption(string message, mpz_t C1_x, mpz_t C1_y, mpz_t C2_x, mpz_t C2_y);
        void ECC_Decryption(mpz_t C1_x, mpz_t C1_y, mpz_t C2_x, mpz_t C2_y);
        void ECC_Sign(string message, mpz_t _r, mpz_t _s);
        int ECC_Verify_Sign(string message, mpz_t _r, mpz_t _s);
    private:

        gmp_randstate_t _gstate;
        clock_t _time;
        HashAlgorithms hash;
        // RSA  keys
        mpz_t _e_key;
        mpz_t _d_key;
        mpz_t _n_key;
        // ElGamal 
        mpz_t _dl_p_key;
        mpz_t _dl_q_key;
        mpz_t _dl_g_key;
        mpz_t _dl_y_key;
        mpz_t _dl_x_key;
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

        void _ecc_point_addition(mpz_t one_x, mpz_t one_y, mpz_t two_x,mpz_t two_y,mpz_t three_x,mpz_t three_y);
        void _ecc_point_doubling(mpz_t one_x, mpz_t one_y);
        void _ecc_point_doubling(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y);
        void _ecc_point_multiplication(mpz_t _K, mpz_t _ecc_t1, mpz_t _ecc_t2, mpz_t Q_x, mpz_t Q_y);
        int * _ecc_NAF(mpz_t _K);
        void _ecc_cyber_text_decode(mpz_t _mess_x, mpz_t _mess_y, mpz_t _message);
        void _ecc_plain_text_embeded(string message,  mpz_t _mess_x, mpz_t _mess_y);
        string _hex_to_string(string message);
        string _string_to_hex(string message);
};

