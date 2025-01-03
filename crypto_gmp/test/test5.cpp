#include "../crypt.h"

int main() {
    Crypt crypt1;
    //const char* message = "111111111111111122222222222233333333334445" ;
    //string message = "X1111111111111111222222222222333333333344459ddQ" ;
    string message = "hello" ;
    cout << "message: " << message << endl;

    int is_out = 1;

    //crypt1.ECC_Generate_Keys("sm2");
    crypt1.ECC_Generate_Keys("secp256k1");

    mpz_t C1_x; mpz_init(C1_x);
    mpz_t C1_y; mpz_init(C1_y);
    mpz_t C2_x; mpz_init(C2_x);
    mpz_t C2_y; mpz_init(C2_y);

    cout << "encryption: " << endl;
    crypt1.ECC_Encryption(message, C1_x, C1_y, C2_x, C2_y);
    gmp_printf("C1_x: %Zd\n", C1_x);
    gmp_printf("C1_y: %Zd\n", C1_y);
    gmp_printf("C2_x: %Zd\n", C2_x);
    gmp_printf("C2_y: %Zd\n", C2_y);
    cout << "decryption: " << endl;
    crypt1.ECC_Decryption(C1_x, C1_y, C2_x, C2_y);

    mpz_clear(C1_x);
    mpz_clear(C1_y);
    mpz_clear(C2_x);
    mpz_clear(C2_y);

}
