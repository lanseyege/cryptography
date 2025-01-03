#include <iostream>
#include <gmpxx.h>

int main() {
    mpz_t _ecc_p_key;
    mpz_t _ecc_a_key;
    mpz_t _ecc_b_key;
    mpz_t _ecc_n_key;
    mpz_t _ecc_Gx_key;
    mpz_t _ecc_Gy_key;
    
    mpz_init(_ecc_p_key);
    mpz_init(_ecc_a_key);
    mpz_init(_ecc_b_key);
    mpz_init(_ecc_n_key);
    mpz_init(_ecc_Gx_key);
    mpz_init(_ecc_Gy_key);
   
    mpz_set_str(_ecc_p_key, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF", 16);
    mpz_set_str(_ecc_a_key, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC", 16);
    mpz_set_str(_ecc_b_key, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93", 16);
    mpz_set_str(_ecc_n_key, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123", 16);
    mpz_set_str(_ecc_Gx_key, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7", 16);
    mpz_set_str(_ecc_Gy_key, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0", 16);

    gmp_printf("_ecc_p_key: %Zd\n", _ecc_p_key);
    gmp_printf("_ecc_a_key: %Zd\n", _ecc_a_key);
    gmp_printf("_ecc_b_key: %Zd\n", _ecc_b_key);
    gmp_printf("_ecc_n_key: %Zd\n", _ecc_n_key);
    gmp_printf("_ecc_Gx_key: %Zd\n", _ecc_p_key);
    gmp_printf("_ecc_Gy_key: %Zd\n", _ecc_Gy_key);
 
}
