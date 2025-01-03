#include <gmpxx.h>
#include <iostream>
#include <sstream>

using namespace std;

string _string_to_hex(string message) {
    string result = "", _temp;
    cout <<"hex to string size: " << message.size() << endl;
    stringstream _ss;
    for (int i = 0; i < message.size(); i++) {
        _ss << hex << int(message[i]) << endl;
        _ss >> _temp;
        result += _temp;
    }
    return result;
}

int main() {
    mpz_t p; mpz_init(p);
    mpz_t a; mpz_init(a);
    mpz_t b; mpz_init(b);
    mpz_t n; mpz_init(n);
    mpz_t Gx; mpz_init(Gx);
    mpz_t Gy; mpz_init(Gy);
    mpz_t K; mpz_init(K);

    //mpz_set_str(p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF", 16);
    mpz_set_str(p, "ffffffff00000001000000000000000000000000ffffffffffffffffffffffff", 16);
    mpz_set_str(a, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC", 16);
    mpz_set_str(b, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93", 16);
    mpz_set_str(n, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123", 16);
    mpz_set_str(Gx, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7", 16);
    mpz_set_str(Gy, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0", 16);
    mpz_set_ui(K, 30);

    string message = "hello";
    mpz_t _message; mpz_init(_message);
    mpz_t _mess_x; mpz_init(_mess_x);
    mpz_t _mess_y; mpz_init(_mess_y);
    mpz_t _temp_m1; mpz_init(_temp_m1);
    mpz_t _temp_m2; mpz_init(_temp_m2);
    mpz_t _temp_m3; mpz_init(_temp_m3);
    mpz_t _temp_m4; mpz_init(_temp_m4);
    mpz_t _temp_m5; mpz_init(_temp_m5);

    mpz_sub_ui(_temp_m1, p, 1);
    mpz_cdiv_q_ui(_temp_m1, _temp_m1, 2);
    mpz_set_str(_message, _string_to_hex(message).c_str(), 16);
    mpz_mul(_mess_x, K, _message);
    mpz_mod_ui(_temp_m1, p, 4); 
    gmp_printf("remd 4 %Zd\n", _temp_m1);
    return 0;
    int i = 0;
    while(true) {
        mpz_pow_ui(_temp_m2, _mess_x, 3);
        gmp_printf("_temp_m2 1: %Zd\n", _temp_m2);
        mpz_mul(_temp_m3, _mess_x, a);
        gmp_printf("_temp_m3 2: %Zd\n", _temp_m3);
        mpz_add(_temp_m2, _temp_m2, _temp_m3);
        gmp_printf("_temp_m2 3: %Zd\n", _temp_m2);
        mpz_add(_temp_m2, _temp_m2, b);
        gmp_printf("_temp_m2 4: %Zd\n", _temp_m2);
        //mpz_mod(_temp_m3, _temp_m2, p);
        //gmp_printf("_temp_m3 5: %Zd\n", _temp_m3);
        mpz_powm(_temp_m5, _temp_m2, _temp_m1, p);
        if(mpz_cmp_ui(_temp_m5, 0) > 0) {
            cout << "yes" << endl;
            break;
        }
        mpz_sqrtrem(_mess_y, _temp_m4, _temp_m2);
        gmp_printf("remaind %Zd\n", _temp_m4);
        gmp_printf("ress %Zd\n", _mess_y);
        //if (mpz_cmp_ui(_temp_m2, 0) == 0) {
        //    cout <<"perfect 1"<<endl;
        //    break;
        //}
       if ( mpz_perfect_square_p(_temp_m2) != 0 )  {
            cout <<"perfect 2"<<endl;
            mpz_sqrt(_mess_y, _temp_m2);
            break;
        } i += 1;
        mpz_add_ui(_mess_x, _mess_x, 1);
        gmp_printf("_mess_x 6: %Zd\n", _mess_x);
        gmp_printf("_prime: %Zd\n", p);
    }
    mpz_clear(_message);
    mpz_clear(_mess_x);
    mpz_clear(_mess_y);
    mpz_clear(_temp_m2);
    mpz_clear(_temp_m3);

}
