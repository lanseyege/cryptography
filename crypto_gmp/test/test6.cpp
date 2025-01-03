#include "../crypt.h"

int main() {
    Crypt crypt1;

    string message = "hello";
    cout << "message: " << message << endl;

     //crypt1.ECC_Generate_Keys("sm2");
    crypt1.ECC_Generate_Keys("secp256k1");
   
    mpz_t _r; mpz_init(_r);
    mpz_t _s; mpz_init(_s);

    cout << "sign ..." << endl;
    crypt1.ECC_Sign(message, _r, _s);

    gmp_printf(" _r: %Zd\n", _r);
    gmp_printf(" _s: %Zd\n", _s);
    
    cout << "verify sign ..." << endl;
    int res = crypt1.ECC_Verify_Sign(message, _r, _s);
    if (res == 0) cout <<"verification failed!!" << endl;
    else cout << "verification acc!!" << endl;

}
