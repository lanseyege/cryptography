#include <iostream>
#include <string>
#include <stdio.h>
#include <bitset>
#include <cstring>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>

using namespace std;

#define MAX_LEN 100000
#define PRIME_AMOUNT 9592
#define BASE 10
#define HASH_LEN 64
#define xfree(x) free(x); x = NULL;


class BigInt{
    
    public:
        BigInt();
        ~BigInt();
        string multiply(string left, string right);
        string add(string left, string right);
        string sub(string left, string right);
        void division(string left, string right);
        void modulo(string left, string right);
        string get_division_value();
        string get_modulo_value();
        int compare(string left, string right);
        string extended_euclidean( string left, string right );
        string modulo_inverse( string left, string right );
        string generate_prime(int prime_len);
        string _modular_pow(string base, string exponent, string modulus);

    protected:
        string _dec2hex(unsigned int _temp, int _len);
        string _hex2dec(string _temp);
        string small_multiply(string left, string right);
        string small_multiply(string left, string right, int left_len, int right_len);
        string pos_sub(string left, string right);
        string res_to_string(string res, int base);
        string karatsuba(string left, string right); 
        string plus(string left, string right);
        void div(string left, string right);
        string get_substr(string temp, int l_pos, int r_pos);

    private:
        string division_value;
        string modulo_value;
        string _BASE;
        string _factor_s;
        string _factor_d;
        string _bottom_int[7] = {"2", "325", "9375", "28178", "450775", "9780504", "1795265022"};
        string _prime_file_name = "../data/primes-100k.dat";
        string _stored_primes[PRIME_AMOUNT];
        int _is_get_stored_primes_; 

        int _miller_rabbin(string _prime);
        void _factor(string _number, string _power);
        string _generate_prime(int prime_len);
        void _get_stored_primes();
        int _prime_fast_test(string _prime );
        int _is_get_stored_primes();

};

class HashAlgorithms : public BigInt{
    public:
        //HashAlgorithms() : BigInt() {};
        //~HashAlgorithms();
        using BigInt::BigInt;
        //using BigInt::~BigInt();

        void MD5_init();
        void sha_1_init();

        string MD5_value(string message);

    private:
        int MD5_S[HASH_LEN], MD5_K[HASH_LEN];
        int MD5_a0, MD5_b0, MD5_c0, MD5_d0;

        //int * _add_char(char *message);
        void _add_char( int *new_message, string message, int new_len, int len, int fill_byte);
        int * _add_char2( string message);
        int _left_shift(int a, int s);
        void _get_MD5_K() ;

        void _little_endian(int *data, int len) ;
};

class RSA : public HashAlgorithms {
    public :
        //using BigInt::BigInt;
        void RSA_Plain_Generate_Keys(int p_length, int q_length);
        string RSA_Plain_Encryption(string message);
        string RSA_Plain_Decryption(string C);
        string RSA_Plain_Sign(string message) ;
        int RSA_Plain_Verify_Sign(string message, string signature);
    private :
        string _stored_e_keys[1] = {"65537"} ;
        string _public_keys[2] ;
        string _private_keys[2];

};

