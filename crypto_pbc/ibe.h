#include <iostream>
#include <gmp.h>
#include <pbc/pbc.h>
#include <cryptopp/sm3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <math.h>
#include <cstddef>
#include <malloc.h>
#include <stdlib.h>
#include "sm4.h"

using namespace std;
using namespace CryptoPP;

class IBE{
    public :
        IBE();
        ~IBE();
    private :
        void ibe_setup();
        void ibe_extract();
        void ibe_encrypt();
        void ibe_decrypt();
        
        int _bn_n, _bn_xp1, _bn_yp1, _bn_xp2, _bn_yp2;
        int _bn_q, _bn_cf, _bn_k, _bn_beta;
        int _bn_private_key;
};

class BasicIdent{
    public :
        BasicIdent();
        ~BasicIdent();
        void basic_ibe_setup();
        void basic_ibe_extract();
        void basic_ibe_encrypt();
        void basic_ibe_decrypt();

    private:
        int _basic_ibe_k;
        clock_t _time;
        gmp_randstate_t _gstate;
        mpz_t _basic_ibe_q;
};

class SM9{
    public :
        SM9();
        ~SM9();
        /** flag = 0, sm9 reset; = 1, random; **/
        void sm9_setup(int flag, string _ids, string _ida); 
        void sm9_extract();
        string sm9_encrypt(string message);
        string sm9_decrypt(string cyber_text);
        void sm9_sign(string message);
        void sm9_verify(string message, string h_hex, string _sx_hex, string _sy_hex);

        string get_h() {return _h_sign_hex;}
        string get_sx() {return _s_sign_hex_x;}
        string get_sy() {return _s_sign_hex_y;}
    private :
        SM3 sm3;
        
        int _flag ;
        int _c1x_len, _c1y_len, _c2_len, _c3_len, _mess_len;
        string ids, ida, message;
        byte hid, hid_sign;
        
        string _h_sign_hex, _s_sign_hex_x, _s_sign_hex_y;

        pairing_t pairing;
        element_t P1, P2;
        element_t public_key, secret_key, secret_skey, public_skey;
        element_t de_B, ds_A;
        element_t _random, _random_sign; 
        element_t h1, h1_sign;

        char * _to_char(const char *strings) ;
        byte * _to_byte(string ids) ;
        byte * _get_element_byte(string _element);
        string _get_element_str(element_t _temp_t, mpz_t _h1);
        void _get_h1(SM3 sm3, string ids, byte hid, mpz_t r, mpz_t _res);
        void _get_h2(SM3 sm3, string message_sign, string _omega_hex, mpz_t r, mpz_t _res);
        void _get_hash(byte *_message, int blen, byte _ct, mpz_t r, mpz_t _res);
        string _get_kdf(SM3 sm3, byte _zinfo[], int zlen, int klen) ;
        string _get_mac(SM3 sm3, string k2, string c2) ;
        string _get_kdf_str(element_t _omega);       
        string _get_str(string _message_hex);
};
