#include "ibe.h"

SM9::SM9(){}

SM9::~SM9(){}

byte * SM9::_to_byte(string ids) {
    int n = ids.length();
    byte * _res = new byte[n];
    for(int i = 0; i < n; i++) 
        _res[i] = ids.at(i);
    return _res;
}

byte * SM9::_get_element_byte(string _element ) {
    int n = _element.length();
    int nb = n;
    if (nb % 2 == 1) nb += 1;
    nb /= 2;
    byte * _element_b = new byte[nb];
    for (int i = 0; i < n; i+= 2 )
        _element_b[i/2] = stoi(_element.substr(i, 2).c_str(), 0, 16);
    return _element_b;
}

string SM9::_get_str(string _message_hex) {
    int _mess_len_hex = _message_hex.length();
    char *_message = new char[_mess_len_hex];
    for (int i = 0; i < _message_hex.length(); i += 2) {
        _message[i/2] = stoi(_message_hex.substr(i, 2).c_str(), 0, 16);
    }
    return _message;
}

string SM9::_get_mac(SM3 sm3, string k2, string c2) {
    string zk = c2 + k2; int _zk_len = zk.length();
    if (_zk_len % 2 == 1) _zk_len ++; _zk_len /= 2;
    byte * _zmessage = _get_element_byte(zk);
    string hash_value;
    StringSource s2(_zmessage, _zk_len, true, new HashFilter(sm3, new HexEncoder(new StringSink(hash_value))));
    return hash_value;
}

void SM9::sm9_setup(int flag, string _ids, string _ida) {
    ids = _ids; ida = _ida; 
    hid = 0x03, hid_sign = 0x01;
    _flag = 1;   

    char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    pairing_init_set_buf(pairing, param, count);
    element_init_G1(P1, pairing);
    element_init_G2(P2, pairing);
    element_init_G1(public_key, pairing);
    element_init_Zr(secret_key, pairing);
    element_init_G2(de_B, pairing);   
    element_init_G1(ds_A, pairing);   
    element_init_Zr(h1, pairing);
    element_init_Zr(h1_sign, pairing);
    element_init_Zr(_random, pairing);
    element_init_Zr(_random_sign, pairing);
    element_init_Zr(secret_skey, pairing);
    element_init_G2(public_skey, pairing);

    if (flag) {
        const char * _secret_key = "01EDEE3778F441F8DEA3D9FA0ACC4E07EE36C93F9A08618AF4AD85CEDE1C22";
        const char * _P1 = "[66882264091090507626018961359014674407194165142668785782120083507098121854429,15376082189538387440900211051596494061397537732182077032922339670147106514454]";
        const char *_P2_x1 = "85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141";
        const char *_P2_x2 = "3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B";
        const char *_P2_y1 = "17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96";
        const char *_P2_y2 = "A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7";
        const char * __random = "AAC0541779C8FC45E3E2CB25C12B5D2576B2129AE8BB5EE2CBE5EC9E785C";
        const char * __random_sign = "033C8616B06704813203DFD00965022ED15975C662337AED648835DC4B1CBE";
        const char * _sign_key = "0130E78459D78545CB54C587E02CF480CE0B66340F319F348A1D5B1F2DC5F4";
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
        element_set_str(P1, _P1, 10);
        element_set_str(P2, _P2, 0);
        if (_flag) element_printf("P2: %B\n", P2);

        mpz_t _temp ; mpz_init(_temp);
        mpz_set_str(_temp, _secret_key, 16);
        element_set_mpz(secret_key, _temp);
        if (_flag) element_printf("secret_key: %B\n", secret_key);

        mpz_set_str(_temp, __random, 16);
        element_set_mpz(_random, _temp);

        mpz_set_str(_temp, __random_sign, 16);
        element_set_mpz(_random_sign, _temp);

        mpz_set_str(_temp, _sign_key, 16);
        element_set_mpz(secret_skey, _temp);

        mpz_clear(_temp_x1);
        mpz_clear(_temp_x2);
        mpz_clear(_temp_y1);
        mpz_clear(_temp_y2);
        mpz_clear(_temp);
    }
    else {
        element_random(P2);
        element_random(P1);
        element_random(secret_key);
        element_random(_random);
        element_random(_random_sign);
        element_random(secret_skey);
    }
}

void SM9::_get_hash(byte *_message, int blen, byte _ct, mpz_t r, mpz_t _res) {
    int ct = 0x00000001;
    int hlen = 320, v = 256;
    int hlen2 = ceil (1.0 * hlen / v);
    string hash_value[hlen2];
    _message[blen - 1] = (byte) (ct & 0xff);
    _message[blen - 2] = (byte) ((ct & 0xff00) >> 8);
    _message[blen - 3] = (byte) ((ct & 0xff0000) >> 16);
    _message[blen - 4] = (byte) ((ct >> 24) & 0xff);
    for (int i = 0; i < blen; i++)
        printf("%2X", _message[i]);
    cout << endl;
    for (int i = 0; i < hlen2; i++) {
        StringSource s2(_message, blen, true, new HashFilter(sm3, new HexEncoder(new StringSink(hash_value[i]))));
        ct ++;
        _message[blen - 1] = (byte) (ct & 0xff);
        _message[blen - 2] = (byte) ((ct & 0xff00) >> 8);
        _message[blen - 3] = (byte) ((ct & 0xff0000) >> 16);
        _message[blen - 4] = (byte) ((ct >> 24) & 0xff);
        cout <<"sm3 : " << hash_value[i] << endl;
    }
    string _res_string ;
    if (hlen % v == 0) 
        _res_string = hash_value[(int) ceil(1.0 * hlen / v) - 1];
    else {
        int _left = (hlen - v * (int) floor(1.0 * hlen / v) ) / 4;
        _res_string = hash_value[(int) ceil(1.0 * hlen / v) - 1].substr(0, _left);}
    string res_string;
    for (int i = 0; i < hlen2 - 1; i++)
        res_string += hash_value[i];
    res_string += _res_string;

    mpz_set_str(_res, res_string.c_str(), 16);
    mpz_t temp; mpz_init(temp);
    mpz_sub_ui(temp, r, 1);
    mpz_mod(_res, _res, temp);
    mpz_add_ui(_res, _res, 1);
    mpz_clear(temp);
}

void SM9::_get_h1(SM3 sm3, string ids, byte hid, mpz_t r, mpz_t _res) {
    byte _ct{0x01}; 

    byte * _bres = _to_byte(ids);
    int _blen = ids.length();
    int blen = 1 + _blen + 1 + 4; 
    byte * _message = new byte[blen];
    _message[0] = _ct ;
    for (int i = 0; i < _blen; i++) 
        _message[i + 1] = _bres[i];
    _message[_blen + 1] = hid; 
    _get_hash(_message, blen, _ct, r, _res);
}

void SM9::_get_h2(SM3 sm3, string message_sign, string _omega_hex, mpz_t r, mpz_t _res) {
    cout <<"message_sign: " << message_sign << endl;
    cout <<"omega_hex : " << _omega_hex << endl;
    byte _ct{0x02}; 
    int mess_len = message_sign.length();
    int _omeg_len = _omega_hex.length();
    if (_omeg_len % 2 == 1) _omeg_len ++;
    _omeg_len /= 2;
    int blen = 1 + mess_len + _omeg_len + 4;
    cout << "mess_len: " << mess_len <<" _omeg len: " << _omeg_len << " blen: " << blen << endl;
    byte * _mess_byte = _to_byte(message_sign);
    for (int i = 0; i < mess_len; i++) 
        printf("%X", _mess_byte[i]);
    cout << endl;
    byte * _omega_byte = _get_element_byte(_omega_hex);
    for (int i = 0; i < _omeg_len; i++) 
        printf("%2X", _omega_byte[i]);
    cout << endl;
    byte * _message = new byte[blen];
    _message[0] = _ct;
    for (int i = 1; i < mess_len + 1; i++)
        _message[i] = _mess_byte[i - 1];
    for (int i = 1; i < _omeg_len + 1; i++)
        _message[i + mess_len] = _omega_byte[i - 1] ;
    _get_hash(_message, blen, _ct, r, _res);
}

string SM9::_get_kdf(SM3 sm3, byte _zinfo[], int zlen, int klen) {
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
    return res_string;
}

string SM9::_get_element_str(element_t _temp_t, mpz_t _h1) {
    char _temp_c[1000];
    element_to_mpz(_h1, _temp_t); 
    mpz_get_str(_temp_c, 16, _h1); 
    string _temp_s(_temp_c);
    cout << _temp_s << endl;
    return _temp_s;   
}

string SM9::_get_kdf_str(element_t _omega) {
    int en = element_item_count(_omega);
    string _res = "";
    mpz_t _h1; mpz_init(_h1);
    for (int i = 0; i < en; i++ ) {
        _res += _get_element_str(element_item(element_item(_omega, i), 1), _h1);
        _res += _get_element_str(element_item(element_item(_omega, i), 0), _h1);
    }
    mpz_clear(_h1);
    return _res;
}

void SM9::sm9_extract() {
    /** generate public key: for encrypt and sign **/
    element_mul_zn(public_key, P1, secret_key);
    element_mul_zn(public_skey, P2, secret_skey);
    if (_flag) element_printf("public_key: %B\n", public_key);
    if (_flag) element_printf("public_skey: %B\n", public_skey);
    
    /** get de_B , decrypt key **/
    mpz_t _h1; mpz_init(_h1);
    _get_h1(sm3, ids, hid, pairing->r, _h1);
    if (_flag) gmp_printf("_h1: %ZX\n", _h1);
    element_set_mpz(h1, _h1);
    element_t t1; element_init_Zr(t1, pairing);
    element_t t2; element_init_Zr(t2, pairing);

    element_add(t1, h1, secret_key);
    element_invert(t2, t1);
    element_mul(t2, secret_key, t2);
    element_mul_zn(de_B, P2, t2);
    if (_flag) element_printf("de_B: %B\n", de_B);
    
    //mpz_clear(_h1);
    element_clear(t1);
    element_clear(t2);
    
    /** get ds_A , verify_sign key **/ 
    _get_h1(sm3, ida, hid_sign, pairing->r, _h1);
    gmp_printf("_h1: %ZX\n", _h1);
    element_set_mpz(h1_sign, _h1);
    element_t t1_sign; element_init_Zr(t1_sign, pairing);
    element_t t2_sign; element_init_Zr(t2_sign, pairing);
    
    element_add(t1_sign, h1_sign, secret_skey);
    element_invert(t2_sign, t1_sign);
    element_mul(t2_sign, secret_skey, t2_sign);
    element_mul_zn(ds_A, P1, t2_sign);
    if (_flag) element_printf("ds_A: %B\n", ds_A);

    mpz_clear(_h1);
    element_clear(t1_sign);
    element_clear(t2_sign);
}

string SM9::sm9_encrypt(string message) {
    _mess_len = message.length();
    /** step 1: **/
    element_t Q_B; element_init_G1(Q_B, pairing);
    element_mul_zn(Q_B, P1, h1);
    element_add(Q_B, Q_B, public_key);
    if (_flag) element_printf("Q_B: %B\n", Q_B);
    /** step 2: **/
    _random;

    /** step 3: **/
    element_t C1; element_init_G1(C1, pairing);
    element_mul_zn(C1, Q_B, _random);
    if (_flag) element_printf("C1: %B\n", C1);

    /** step 4: **/
    element_t GT_g; element_init_GT(GT_g, pairing);
    //element_pairing(GT_g, public_key, P2);
    pairing_apply(GT_g, public_key, P2, pairing);
    if (_flag) element_printf("GT_g: %B\n", GT_g);

    /** step 5: **/
    element_t _omega; element_init_GT(_omega, pairing);
    element_pow_zn(_omega, GT_g, _random);
    if (_flag) element_printf("_omega: %B\n", _omega);

    /** step 6 - 1: **/
    mpz_t _h1; mpz_init(_h1);
    string _kdf_str = _get_kdf_str(_omega);
    string C1_x = _get_element_str(element_item(C1, 0), _h1);
    string C1_y = _get_element_str(element_item(C1, 1), _h1);
    _c1x_len = C1_x.length();
    _c1y_len = C1_y.length();
    _kdf_str = C1_x + C1_y + _kdf_str;

    byte * _ids = _to_byte(ids);
    byte * _zin = _get_element_byte(_kdf_str);

    int zlen = _kdf_str.length() , _zlen;
    if (zlen % 2 == 1) zlen += 1;
    zlen /= 2; _zlen = zlen; zlen += ids.length();

    byte * _zinfos = new byte[zlen];
    for (int i = 0; i < zlen; i++ ) {
        if (i < _zlen) _zinfos[i] = _zin[i];
        else _zinfos[i] = _ids[i - _zlen];
    }
    int k1_len = 128, k2_len = 0x100;
    string _res_kdf = _get_kdf(sm3, _zinfos, zlen, k1_len + k2_len);
    string _k1 = _res_kdf.substr(0, k1_len / 4);
    string _k2 = _res_kdf.substr(k1_len / 4, k2_len / 4);
    if (_flag) cout << "_res_kdf: " << _res_kdf << endl;
    if (_flag) cout << "_k1: " << _k1 << endl;
    if (_flag) cout << "_k2: " << _k2 << endl;

    /** step 6 - 2: **/
    SM4 *sm4 = new SM4();
    sm4->pk_exte(_k1);
    string en_message = sm4->_string_to_hex(message);
    string C2 = sm4->en_de_crypt(en_message, 0);
    string _decyber = sm4->en_de_crypt(C2, 1);
    _c2_len = C2.length();
    if (_flag) cout <<"message hex: " << en_message << endl;
    if (_flag) cout <<"C2: " << C2 << endl;
    if (_flag) cout <<"decyber: " << _decyber << endl;

    /** step 7: **/
    string C3 = _get_mac(sm3, _k2, C2);
    _c3_len = C3.length();

    /** step 8: **/
    string final_cyber = C1_x + C1_y + C3 + C2;

    element_clear(C1);
    element_clear(GT_g);
    element_clear(Q_B);
    mpz_clear(_h1);

    return final_cyber;
}

string SM9::sm9_decrypt(string cyber_text) {

    /** step 1: **/
    string _c1_x = cyber_text.substr(0, _c1x_len);
    string _c1_y = cyber_text.substr(_c1x_len, _c1y_len);
    string _c2 = cyber_text.substr(_c1x_len+_c1y_len+_c3_len, _c2_len);
    string _c3 = cyber_text.substr(_c1x_len+_c1y_len, _c3_len) ;
    if (_flag) cout <<"_c2: " << _c2 << endl;
    if (_flag) cout <<"_c3: " << _c3 << endl;

    element_t _C1; element_init_G1(_C1, pairing);
    char _buf1[1000], _buf2[1000];
    mpz_t _temp_c1x; mpz_init(_temp_c1x);
    mpz_t _temp_c1y; mpz_init(_temp_c1y);
    mpz_set_str(_temp_c1x, _c1_x.c_str(), 16);
    mpz_set_str(_temp_c1y, _c1_y.c_str(), 16);
    gmp_sprintf(_buf1, "%Zd", _temp_c1x);
    gmp_sprintf(_buf2, "%Zd", _temp_c1y);
    element_set_str(_C1, ("["+string(_buf1)+","+string(_buf2)+"]").c_str(), 0);
    if (element_is0(_C1)) {return "";}

    /** step 2: **/
    element_t _omega_p; element_init_GT(_omega_p, pairing);
    element_pairing(_omega_p, _C1, de_B);

    /** step 3 - 1: **/
    int k1_len = 0x80, k2_len = 0x100, mlen = _c2_len;
    string _kdf_str = _get_kdf_str(_omega_p) ;
    _kdf_str = _c1_x + _c1_y + _kdf_str;

    byte * _ids = _to_byte(ids);
    byte * _zin = _get_element_byte(_kdf_str);
    int zlen = _kdf_str.length(), _zlen;
    if (zlen % 2 == 1) zlen += 1;
    zlen /= 2; _zlen = zlen; zlen += ids.length();

    byte * _zinfos = new byte[zlen];
    for (int i = 0; i < zlen; i++) 
        if (i < _zlen) _zinfos[i] = _zin[i] ;
        else _zinfos[i] = _ids[i - _zlen];
    string _res_kdf = _get_kdf(sm3, _zinfos, zlen, k1_len + k2_len);
    string _k1_p = _res_kdf.substr(0, k1_len / 4);
    string _k2_p = _res_kdf.substr(k1_len / 4, k2_len / 4);
    if (_flag) cout << "_res_kdf: " << _res_kdf << endl;
    if (_flag) cout << "_k1_p: " << _k1_p << endl;
    if (_flag) cout << "_k2_p: " << _k2_p << endl;

    /** step 3 - 2: **/
    SM4 *sm4 = new SM4();
    sm4->pk_exte(_k1_p);
    string _message_p = sm4->en_de_crypt(_c2, 1); 

    /** step 4: **/
    string _uc3 = _get_mac(sm3, _k2_p, _c2) ;
    if (_flag) cout <<"_uc3: " << _uc3 << endl;
    if (_c3.compare(_uc3) == 0) {cout <<"True.."<<endl;}
    else cout <<"Wrong..."<<endl;

    element_clear(_C1);
    element_clear(_omega_p);

    return _get_str(_message_p);
}

void SM9::sm9_sign(string message) {
    /** step 1: **/  
    element_t _sign_g; element_init_GT(_sign_g, pairing);
    element_pairing(_sign_g, P1, public_skey);
    if (_flag) element_printf("sign_g: %B\n", _sign_g);

    /** step 2: **/ 
    _random_sign;

    /** step 3: **/
    element_t _omega_sign; element_init_GT(_omega_sign, pairing);
    element_pow_zn( _omega_sign, _sign_g, _random_sign);
    if (_flag) element_printf("omega_sign: %B\n", _omega_sign);

    /** step 4: **/
    mpz_t _h_sign; mpz_init(_h_sign);
    element_t _h_s; element_init_Zr(_h_s, pairing);
    string _omega_str = _get_kdf_str(_omega_sign);
    _get_h2(sm3, message, _omega_str, pairing->r, _h_sign);
    element_set_mpz(_h_s, _h_sign);
    if (_flag) element_printf("h_s : %B\n", _h_s);

    /** step 5: **/
    element_t _l; element_init_Zr(_l, pairing);
    element_sub(_l, _random_sign, _h_s);
    if (_flag) element_printf("l: %B\n", _l);
    
    /** step 6: **/
    element_t _s; element_init_G1(_s, pairing);
    element_mul_zn( _s, ds_A, _l);
    if (_flag) element_printf("s: %B\n", _s);

    /** step 7: **/
    char _h_sign_ch[1000];
    mpz_get_str(_h_sign_ch, 16, _h_sign); _h_sign_hex = _h_sign_ch;
    _s_sign_hex_x = _get_element_str(element_item(_s, 0), _h_sign);
    _s_sign_hex_y = _get_element_str(element_item(_s, 1), _h_sign);

    mpz_clear(_h_sign);
    element_clear(_sign_g);
    element_clear(_omega_sign);
    element_clear(_h_s);
    element_clear(_l);
    element_clear(_s);

}

void SM9::sm9_verify(string message, string h_hex, string _sx_hex, string _sy_hex) {
    /** step 1: **/
    mpz_t h_prime; mpz_init(h_prime);
    mpz_set_str(h_prime, h_hex.c_str(), 16);
    if (mpz_cmp(h_prime, pairing->r) > 0) {
        cout <<"sign h wrong !!!"<< endl;
        return;
    }
    element_t _h_prime; element_init_Zr(_h_prime, pairing);
    element_set_mpz(_h_prime, h_prime);
    if (_flag) element_printf("_h_prime: %B\n", _h_prime);

    /** step 2: **/
    mpz_t sx_prime; mpz_init(sx_prime);
    mpz_t sy_prime; mpz_init(sy_prime);
    mpz_set_str(sx_prime, _sx_hex.c_str(), 16);
    mpz_set_str(sy_prime, _sy_hex.c_str(), 16);
    char _buf1[1000], _buf2[1000];
    gmp_sprintf(_buf1, "%Zd", sx_prime);
    gmp_sprintf(_buf2, "%Zd", sy_prime);
    element_t s_prime; element_init_G1(s_prime, pairing);
    element_set_str(s_prime, ("["+string(_buf1)+","+string(_buf2)+"]").c_str(), 10);
    if (_flag) element_printf("s_prime: %B\n", s_prime);

    /** step 3: **/
    element_t g_prime; element_init_GT(g_prime, pairing);
    element_pairing(g_prime, P1, public_skey);
    if (_flag) element_printf("g_prime: %B\n", g_prime);

    /** step 4: **/
    element_t t_prime; element_init_GT(t_prime, pairing);
    element_pow_zn(t_prime, g_prime, _h_prime);
    if (_flag) element_printf("t_prime: %B\n", t_prime);

    /** step 5: **/
    mpz_t h1_prime; mpz_init(h1_prime);
    _get_h1(sm3, ida, hid_sign, pairing->r, h1_prime);
    if (_flag) gmp_printf("h1_prime: %Zd", h1_prime);
    element_t _h1_prime; element_init_Zr(_h1_prime, pairing);
    element_set_mpz(_h1_prime, h1_prime); 
    if (_flag) element_printf("_h1_prime: %B\n", _h1_prime);

    /** step 6: **/
    element_t p_prime; element_init_G2(p_prime, pairing);
    element_mul_zn(p_prime, P2, _h1_prime);
    element_add(p_prime, p_prime, public_skey);
    if (_flag) element_printf("p_prime: %B\n", p_prime);

    /** step 7: **/
    element_t u_prime; element_init_GT(u_prime, pairing);
    element_pairing(u_prime, s_prime, p_prime);
    if (_flag) element_printf("u_prime: %B\n", u_prime);

    /** step 8: **/
    element_t w_prime; element_init_GT(w_prime, pairing);
    element_mul(w_prime, u_prime, t_prime); 
    if (_flag) element_printf("w_prime: %B\n", w_prime);

    /** step 9: **/
    string _w_prime_hex = _get_kdf_str(w_prime);
    mpz_t h2_prime; mpz_init(h2_prime);
    _get_h2(sm3, message, _w_prime_hex, pairing->r, h2_prime);
    gmp_printf("h_prime : %ZX\n", h_prime);
    gmp_printf("h2_prime: %ZX\n", h2_prime);
    if (mpz_cmp(h_prime, h2_prime) == 0) {
        cout << "verify pass!!!" << endl;
    }else{
        cout << "verify failed!!!" << endl;
    }

    mpz_clear(h_prime);
    mpz_clear(sx_prime);
    mpz_clear(sy_prime);
    mpz_clear(h1_prime);
    mpz_clear(h2_prime);
    element_clear(s_prime);
    element_clear(g_prime);
    element_clear(t_prime);
    element_clear(_h_prime);
    element_clear(_h1_prime);
    element_clear(p_prime);
    element_clear(u_prime);
    element_clear(w_prime);
    return ;
}

int main() {
    SM9 sm9; string ids = "Bob", ida = "Alice";
    sm9.sm9_setup(0, ids, ida);
    sm9.sm9_extract();
    string message = "Chinese IBE standard";
    message = "Compared to what we have started with, we now have a robust protocol. However there is still a significant drawback to the zero-knowledge property, regardless of encryption: while theoretically polynomial coefficients ci can have a vast range of values, in reality, it might be quite limited (6 in the previous example), which means that the verifier could brute-force limited range of coefficients combinations until the result is equal to the prover's answer. For instance if we consider the range of 100 values for each coefficient, the degree 2 polynomial would total to 1 million of distinct combinations, which considering brute-force would require less than 1 million iterations. Moreover, the secure protocol should be secure even in cases where there is only one coefficient, and its value is 1.";
    cout << "message: " << message << endl;
    string final_cyber = sm9.sm9_encrypt(message);
    cout << "final_cyber: " << final_cyber << endl;

    string plain_text = sm9.sm9_decrypt(final_cyber);
    cout << "plain text: " << plain_text << endl; 

    sm9.sm9_sign(message);
    cout << "sign h value: " << sm9.get_h() << endl;
    cout << "sign sx value: " << sm9.get_sx() << endl;
    cout << "sign sy value: " << sm9.get_sy() << endl;

    sm9.sm9_verify(message, sm9.get_h(), sm9.get_sx(), sm9.get_sy());
}

