#include "t9.h"

int main() {
    Crypt crypt1;
    //const char* message = "111111111111111122222222222233333333334445" ;
    //string message = "X1111111111111111222222222222333333333344459ddQ" ;
    string message = "0" ;
    cout << "message: " << message << endl;

    int is_out = 1;
    //crypt1.Ecc_Init("s");
    crypt1.ECC_Generate_Keys("s");
    //crypt1.ECC_Generate_Keys("secp256k1");

    mpz_t C1_x; mpz_init(C1_x);
    mpz_t C1_y; mpz_init(C1_y);
    mpz_t C2_x; mpz_init(C2_x);
    mpz_t C2_y; mpz_init(C2_y);
    
    mpz_t _temp_x; mpz_init(_temp_x);
    mpz_t _temp_y; mpz_init(_temp_y);
    mpz_t _temp_C1_y; mpz_init(_temp_C1_y);
    mpz_t _temp_C1_x; mpz_init(_temp_C1_x);

    mpz_set_ui(C1_x, 3);
    mpz_set_ui(C1_y, 10);
    mpz_set_ui(C2_x, 9);
    mpz_set_ui(C2_y, 7);

    crypt1._ecc_point_addition(C1_x, C1_y, C2_x, C2_y, _temp_x, _temp_y);

    mpz_mul_si(_temp_C1_y, C1_y, -1);
    mpz_mod(_temp_C1_y, _temp_C1_y, crypt1._ecc_p);
    gmp_printf("_temp_C1_y: %Zd\n", _temp_C1_y); 
    //crypt1.ECC_Encryption(message, C1_x, C1_y, C2_x, C2_y);
    gmp_printf("C1_x: %Zd\n", C1_x);
    gmp_printf("C1_y: %Zd\n", C1_y);
    gmp_printf("C2_x: %Zd\n", C2_x);
    gmp_printf("C2_y: %Zd\n", C2_y);
    
    gmp_printf("_temp_x: %Zd\n", _temp_x);
    gmp_printf("_temp_y: %Zd\n", _temp_y);
    mpz_t kk1; mpz_init(kk1);
    mpz_t kk2; mpz_init(kk2);
    crypt1._ecc_point_addition(_temp_x, _temp_y, C1_x, _temp_C1_y, kk1, kk2);
    gmp_printf("kk1: %Zd\n", kk1);
    gmp_printf("kk2: %Zd\n", kk2);

    int s = crypt1._ecc_point_addition(C1_x, C1_y, C1_x, _temp_C1_y, _temp_x, _temp_y);
    cout << " s: " << s << endl;
    gmp_printf("_temp_x 2: %Zd\n", _temp_x);
    gmp_printf("_temp_y 2: %Zd\n", _temp_y); 

    //crypt1.ECC_Decryption(C1_x, C1_y, C2_x, C2_y);
    
    mpz_set(_temp_C1_x, C1_x);
    mpz_set(_temp_C1_y, C1_y);
    crypt1._ecc_point_doubling(_temp_C1_x, _temp_C1_y);
    gmp_printf("double _temp_C1_x: %Zd\n", _temp_C1_x);
    gmp_printf("double _temp_C1_y: %Zd\n", _temp_C1_y);
    gmp_printf("C1_x: %Zd\n", C1_x);
    gmp_printf("C1_y: %Zd\n", C1_y);
    
    mpz_t k ; mpz_init(k);
    mpz_set_ui(k, 28);
    mpz_set_ui(_temp_C1_x, 0);
    mpz_set_ui(_temp_C1_y, 0);
    crypt1._ecc_point_multiplication(k, C1_x, C1_y, _temp_C1_x, _temp_C1_y);
    gmp_printf("multiple _temp_C1_x: %Zd\n", _temp_C1_x);
    gmp_printf("multiple _temp_C1_y: %Zd\n", _temp_C1_y);
    gmp_printf("C1_x: %Zd\n", C1_x);
    gmp_printf("C1_y: %Zd\n", C1_y); 
 
    mpz_clear(C1_x);
    mpz_clear(C1_y);
    mpz_clear(C2_x);
    mpz_clear(C2_y);
    //mpz_clear(_temp_x);
    //mpz_clear(_temp_y);

}
