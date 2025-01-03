#include <pbc/pbc.h>
#include <gmpxx.h>
#include <iostream>
#include <cryptopp/sm3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <math.h>
#include <cstddef>
#include <malloc.h>
#include <stdlib.h>
#include "../sm4.h"

using namespace std;
using namespace CryptoPP;

//void _print_decide(element_t _temp) {
//    _temp
//}

char * _to_char(const char * strings) {
    int n = strlen(strings); //.length();
    //cout << "n: " << n << endl;
    //unsigned char * res = new unsigned char[n];
    char * res = new char[n];
    for (int i = 0 ; i < n; i++)
        res[i] = strings[i] - '0';
    return res;
}

byte * _to_byte(string ids) {
    int n = ids.length();
    //cout << "n: " << n << endl;
    byte * _res = new byte[n];
    for(int i = 0; i < n; i++) 
        _res[i] = ids.at(i);
    return _res;
}

byte * _get_element_byte(string _element ) {
    int n = _element.length();
    int nb = n;
    if (nb % 2 == 1) nb += 1;
    nb /= 2;
    byte * _element_b = new byte[nb];
    for (int i = 0; i < n; i+= 2 )
        _element_b[i/2] = stoi(_element.substr(i, 2).c_str(), 0, 16);
    return _element_b;
}

string _get_element_str(element_t _temp_t, mpz_t _h1) {
    char _temp_c[1000];
    element_to_mpz(_h1, _temp_t); 
    mpz_get_str(_temp_c, 16, _h1); 
    string _temp_s(_temp_c);
    cout << _temp_s << endl;
    return _temp_s;   
}

void get_h1(SM3 sm3, string ids, byte hid, mpz_t r, mpz_t _res) {
    byte ct[4]{0x00, 0x00, 0x00, 0x01};
    byte _ct{0x01};
    int hlen = 320, v = 256; //8 * ceil(5 * log(2, r) / 32);
    int hlen2 = ceil(320.0 / 256) ;
    string hash_value[hlen2] ;
    //const char * message = "01426f620300000001";
    //char *_message = _to_char(message);
    byte * _bres = _to_byte(ids);
    int _blen =  ids.length(); 
    cout << "_blen: " << _blen << endl;
    int blen = 1 + _blen + 1 + 4;
    byte * message = new byte[blen];
    message[0] = _ct; 
    for (int i = 0; i < _blen; i++) 
        message[i+1] = _bres[i];
    message[_blen+1] = hid;
    message[_blen+2] = ct[0]; message[_blen+3] = ct[1]; message[_blen+4] = ct[2]; message[_blen+5] = ct[3];
    //byte message[9]{0x01, 0x42, 0x6f, 0x62, 0x03, 0x00, 0x00, 0x00, 0x01};
    cout << "message: " << message << endl;
    for (int i = 1; i <= hlen2 ; i++) {
        StringSource s2(message, 9, true, new HashFilter(sm3, new HexEncoder(new StringSink(hash_value[i-1]))));
        cout << "sm3 hash: " << hash_value[i-1] << endl;
        message[blen - 1] += 1;
    }
    string res_string ;
    res_string += hash_value[0];
    res_string += hash_value[1].substr(0, 8*2);
    mpz_set_str(_res, res_string.c_str(), 16);
    mpz_t temp; mpz_init(temp);
    mpz_sub_ui(temp, r, 1);
    mpz_mod(_res, _res, temp);
    mpz_add_ui(_res, _res, 1);
    mpz_clear(temp);
}

string _get_kdf(SM3 sm3, byte _zinfo[], int zlen, int klen ) {
    //byte ct[4] {0x00, 0x00, 0x00, 0x01};
    int ct = 0x00000001 ;
    int v = 256;
    int hlen2 = ceil(1.0 * klen / v);
    cout << "hlen2: " << hlen2 << endl;
    string hash_value[hlen2];
    byte * _zct = new byte[zlen + 4];
    for (int i = 0; i < zlen; i++) 
        _zct[i] = _zinfo[i] ;
    _zct[zlen + 3] = ct & 0xff;
    _zct[zlen + 2] = (byte) ((ct & 0xff00) >> 8); 
    _zct[zlen + 1] = (byte) ((ct & 0xff0000) >> 16);
    _zct[zlen + 0] = (byte) ((ct >> 24 ) & 0xff );
    for (int i = 0; i < hlen2; i++) {
        StringSource s2(_zct, zlen+4, true, new HashFilter(sm3, new HexEncoder(new StringSink(hash_value[i]))));
        //_zct[zlen + 3] += 1; 
        ct ++;
        _zct[zlen + 3] = ct & 0xff;
        _zct[zlen + 2] = (byte) ((ct & 0xff00) >> 8); 
        _zct[zlen + 1] = (byte) ((ct & 0xff0000) >> 16);
        _zct[zlen + 0] = (byte) ((ct >> 24 ) & 0xff );
    }
    string _res_string;
    if (klen % v == 0) {
        _res_string = hash_value[(int)ceil(1.0 * klen/v) - 1];
    } else {
        int _left = (klen - v * floor(1.0 * klen/v) ) / 4;
        _res_string = hash_value[(int)ceil(1.0 * klen/v) - 1].substr(0, _left);
    }
    string res_string;
    for (int i = 0; i < hlen2 - 1; i++)
        res_string += hash_value[i];
    res_string += _res_string;
    //mpz_set_str(_res, res_string.c_str(), 16);
    return res_string;
}

string _get_mac(SM3 sm3, string k2, string c2) {
    string zk = c2 + k2;
    byte * _zmessage = _get_element_byte(zk);
    string hash_value;
    StringSource s2(_zmessage, zk.length(), true, new HashFilter(sm3, new HexEncoder(new StringSink(hash_value))));
    return hash_value;
}

void encrypt() {
    string ids = "Bob";
    byte hid{0x03} ;
    SM3 sm3;
    pairing_t pairing;
    char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error"); 
    pairing_init_set_buf(pairing, param, count);
    element_t P1, P2;
    element_t public_key, secret_key, sign_pk;
    
    element_init_G1(P1, pairing);
    element_init_G2(P2, pairing);
    element_init_G1(public_key, pairing);
    element_init_Zr(secret_key, pairing);
    element_init_G2(sign_pk, pairing);

    element_random(secret_key);
    element_random(P1);
    element_random(P2);
    element_random(sign_pk);

    const char *_secret_key = "01EDEE3778F441F8DEA3D9FA0ACC4E07EE36C93F9A08618AF4AD85CEDE1C22";
    const char *_P1 = "[93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD,21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616]";
    _P1 = "[66882264091090507626018961359014674407194165142668785782120083507098121854429,15376082189538387440900211051596494061397537732182077032922339670147106514454]";
    const char *_P2_x1 = "85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141";
    const char *_P2_x2 = "3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B";
    const char *_P2_y1 = "17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96";
    const char *_P2_y2 = "A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7";
    mpz_t _temp_x1; mpz_init(_temp_x1);
    mpz_t _temp_x2; mpz_init(_temp_x2);
    mpz_t _temp_y1; mpz_init(_temp_y1);
    mpz_t _temp_y2; mpz_init(_temp_y2);
    mpz_set_str(_temp_x1, _P2_x1, 16);
    mpz_set_str(_temp_x2, _P2_x2, 16);
    mpz_set_str(_temp_y1, _P2_y1, 16);
    mpz_set_str(_temp_y2, _P2_y2, 16);
    char _buf1[1024], _buf2[1024], _buf3[1024], _buf4[1024]; 
    gmp_sprintf(_buf1, "%Zd", _temp_x1);
    gmp_sprintf(_buf2, "%Zd", _temp_x2);
    gmp_sprintf(_buf3, "%Zd", _temp_y1);
    gmp_sprintf(_buf4, "%Zd", _temp_y2);
    string _temp_s = string("[[") + string(_buf2) + string(", ") + string(_buf1) + string("], [") + string(_buf4) + string(", ") + string(_buf3) + string("]]");
    const char *_P2  = _temp_s.c_str();
    cout << "_P2: " << _P2 << endl;
    //_P2 = "[[51087946785655159605432701985040347632394126441782508708859971390823966253587, 36036733291017124612136112515704680556400752705984545127885593119377020369897], [11190186047680900869464485595689784471237657870165440799638733221078155282145, 3813334973837722384075482691110345437369355145180959163747982231335491280225]]";
    element_set_str(P1, _P1, 10);
    element_set_str(P2, _P2, 0);
    
    mpz_t _temp ; mpz_init(_temp);
    mpz_set_str(_temp, _secret_key, 16);
    element_set_mpz(secret_key, _temp);
    
    element_mul_zn(public_key, P1, secret_key);

    //element_mul_zn(sign_pk, P2, secret_key);
    //element_printf("sign_pk = %B\n", sign_pk);
    element_printf("P1 = %B\n", P1);
    element_printf("P2 = %B\n", P2);
    element_to_mpz(_temp, secret_key);
    gmp_printf("secret_key = %Zx\n", _temp);
    //gmp_printf("secret_key = %Zd\n", _temp);
    element_printf("secret_key = %B\n", secret_key);
    element_printf("public_key = %B\n", public_key);

    gmp_printf("r: %Zx\n", pairing->r);
    gmp_printf("r: %Zd\n", pairing->r);

    //get_h1(sm3, pairing->r);
    mpz_t _h1; mpz_init(_h1);
    get_h1(sm3, ids, hid, pairing->r, _h1);
    gmp_printf("h1: %Zx\n", _h1);
    element_t h1; element_init_Zr(h1, pairing);
    element_set_mpz(h1, _h1);
    element_t t1; element_init_Zr(t1, pairing);
    element_add(t1, h1, secret_key);
    element_to_mpz(_h1, t1);
    gmp_printf("t1: %Zx\n", _h1);
    
    element_t t2; element_init_Zr(t2, pairing);
    element_invert(t2, t1);
    element_mul(t2, secret_key, t2);
    element_to_mpz(_h1, t2);
    gmp_printf("t2: %ZX\n", _h1);
    element_t de_B; element_init_G2(de_B, pairing);
    element_mul_zn(de_B , P2, t2);
    element_printf("de_B : %B\n", de_B);
    
    
    /** step 1; **/
    element_t Q_B; element_init_G1(Q_B, pairing);
    element_mul_zn(Q_B, P1, h1);
    //element_printf("Q_B 1: %B\n", Q_B);
    element_add(Q_B, Q_B, public_key);
    element_printf("Q_B 2: %B\n", Q_B);
    
    /** step 2 **/
    element_t _random; element_init_Zr(_random, pairing);
    string __random = "AAC0541779C8FC45E3E2CB25C12B5D2576B2129AE8BB5EE2CBE5EC9E785C";
    mpz_t __random_mpz ; mpz_init(__random_mpz);
    mpz_set_str(__random_mpz, __random.c_str(), 16); 
    element_set_mpz(_random, __random_mpz);
    
    /** step 3 **/
    element_t C1; element_init_G1(C1, pairing);
    element_mul_zn(C1, Q_B, _random);
    element_printf("C1 2: %B\n", C1);

    /** step 4 **/
    element_t GT_g; element_init_GT(GT_g, pairing);
    //element_pairing(GT_g , public_key, P2); 
    pairing_apply(GT_g, public_key, P2, pairing);
    element_printf("GT_g: %B\n", GT_g);

    /** step 5 **/
    element_t _omega; element_init_GT(_omega, pairing);
    element_pow_zn( _omega, GT_g, _random);
    element_printf("_omega: %B\n", _omega);

    int en = element_item_count(_omega) ; 
    string _reso = "";
    for (int i = 0; i < en; i++) { 
        _reso += _get_element_str(element_item(element_item(_omega, i), 1), _h1);
        _reso += _get_element_str(element_item(element_item(_omega, i), 0), _h1);
    }
    string C1_x = _get_element_str(element_item(C1, 0), _h1);
    string C1_y = _get_element_str(element_item(C1, 1), _h1);
    _reso = C1_x + C1_y + _reso;
    //_reso = _get_element_str(element_item(C1, 1), _h1) + _reso;
    //_reso = _get_element_str(element_item(C1, 0), _h1) + _reso;

    byte * _ids = _to_byte(ids);
    byte * _zin = _get_element_byte(_reso);
    
    int zlen = _reso.length(), _zlen;
    if (zlen % 2 == 1) zlen += 1;
    zlen /= 2; _zlen = zlen; zlen += ids.length();

    byte * _zinfos = new byte[zlen];
    for (int i = 0; i < zlen; i++) {
        if ( i < _zlen) 
            _zinfos[i] = _zin[i];
        else 
            _zinfos[i] = _ids[i - _zlen];
        printf("%X" , _zinfos[i]);
    }
    cout << endl;
    //cout << _zinfos << endl; 
    /** step 6 **/
    
    int k1_len = 128, k2_len = 0x100;
    string _res_kdf = _get_kdf(sm3, _zinfos, zlen, k1_len + k2_len);
    string _k1 = _res_kdf.substr(0 , k1_len / 4);
    string _k2 = _res_kdf.substr(k1_len / 4 , k2_len / 4);
    cout << "_res_kdf: " << _res_kdf << "  len: " << _res_kdf.length() << endl;
    cout << "_k1: " << _k1 <<" len: " << _k1.length() << endl;
    cout << "_k2: " << _k2 <<" len: " << _k2.length() << endl;
    
    string en_message = "Chinese IBE standard" ;

    SM4 *sm4 = new SM4(); 
    sm4->pk_exte(_k1);
    
    cout << "en message: " << en_message << endl;
    en_message = sm4->_string_to_hex(en_message)  ;
    cout << "en message byte: " << en_message << endl;

    string c2 = sm4->en_de_crypt(en_message, 0); 
    cout << "sm3 crypt: " << c2 << endl;
    string _decyber = sm4->en_de_crypt(c2, 1);
    cout <<"sm3 _decyber: " << _decyber << endl;

    string c3 = _get_mac(sm3, _k2, c2);
    cout <<"c3 : " << c3 << endl;

    string final_cyber = C1_x + C1_y + c3 + c2;
    cout << "SM9 final cyber: " << final_cyber << endl;

    element_clear(P1);
    element_clear(P2);
    element_clear(public_key);
    element_clear(secret_key); 
    element_clear(h1);
    element_clear(t1);
    element_clear(t2);
    element_clear(Q_B);
    element_clear(C1);
    element_clear(GT_g);
    element_clear(_omega);

    mpz_clear(_temp_x1);
    mpz_clear(_temp_x2);
    mpz_clear(_temp_y1);
    mpz_clear(_temp_y2);
    mpz_clear(_temp);
    mpz_clear(_h1);

}

void decrypt(string final_cyber) {
    int mlen = final_cyber.length(), k1_len = 0x80, k2_len = 0x100;
    string ID_B = "Bob"; 

}

int main() {
    encrypt();
    decrypt();
}
