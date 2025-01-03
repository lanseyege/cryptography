#include "../crypt.h"


int main() {
    Crypt crypt1;
    const char* message = "111111111111111122222222222233333333334" ;
    cout << "message: " << message << endl;

    int p_len = 511, q_len = 650, is_out = 1;
    crypt1.DL_Generate_Keys(q_len, is_out);
    mpz_t _C1; mpz_init(_C1);
    mpz_t _C2; mpz_init(_C2);
    crypt1.DL_ElGamal_Encryption(message, _C1, _C2, is_out);
    mpz_t _M; mpz_init(_M);
    crypt1.DL_ElGamal_Decryption(_C1, _C2, _M, is_out);

    mpz_clear(_C1);
    mpz_clear(_C2);
    mpz_clear(_M);
}
