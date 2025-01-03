#include "../crypt.h"
//#include 

int main () {
    Crypt crypt1;
    //string message = "11111111111111112222222222223333333333" ;
    const char* message = "11111111111111112222222222223333333333" ;
    cout << "message: " << message << endl;
    int p_len = 511, q_len = 650, is_out = 1;
    crypt1.RSA_Plain_Generate_Keys(p_len, q_len, is_out);
    cout << "generate keys" << endl; 
    mpz_t res; mpz_init(res);
    mpz_t sou; mpz_init(sou);
    crypt1.RSA_Plain_Encryption(message, res);
    gmp_printf("encrypt: %Zd\n", res);
    crypt1.RSA_Plain_Decryption(res, sou);
    gmp_printf("decrypt: %Zd\n", sou);

    mpz_clear(res); mpz_clear(sou);
    return 0;
}
