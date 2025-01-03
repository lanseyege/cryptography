#include "../crypt.h"


int main() {
    Crypt crypt1;
    const char* message = "111111111111111122222222222233333333334445" ;
    cout << "message: " << message << endl;

    int p_len = 511, q_len = 650, is_out = 1;
    crypt1.DL_Generate_Keys(q_len, is_out);
    mpz_t _R; mpz_init(_R);
    mpz_t _S; mpz_init(_S);
    crypt1.DL_ElGamal_Sign(message, _R, _S, is_out);

    crypt1.DL_ElGamal_Verify_Sign(message, _R, _S, is_out);

    mpz_clear(_R);
    mpz_clear(_S);
}
