#include "../crypt.h"

int main () {
    Crypt crypt1;
    const char* message = "11111111111111112222222222223333333333" ;
    cout << "message: " << message << endl;


    int p_len = 511, q_len = 650, is_out = 1;
    crypt1.RSA_Plain_Generate_Keys(p_len, q_len, is_out);
    cout << "generate keys" << endl; 

    mpz_t res; mpz_init(res);
    crypt1.RSA_Plain_Sign(message, res, is_out);

    //crypt1.RSA_Plain_Encryption(message, res);
    gmp_printf("encrypt: %Zd\n", res);
    int acc = crypt1.RSA_Plain_Verify_Sign(message, res, is_out) ;
    
    mpz_clear(res); 
    return 0;
}
