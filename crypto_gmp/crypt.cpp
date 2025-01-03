#include "crypt.h"

Crypt::Crypt() {
    mpz_init(_e_key);
    mpz_init(_d_key);
    mpz_init(_n_key);

    mpz_init(_dl_p_key);
    mpz_init(_dl_q_key);
    mpz_init(_dl_g_key);
    mpz_init(_dl_y_key);
    mpz_init(_dl_x_key);
    prime_test_reps = 40;

    mpz_set_str(_e_key, "65537", 10);
    _time = clock();
    gmp_randinit_default(_gstate);
    gmp_randseed_ui(_gstate, _time); 

    mpz_init(_ecc_p);
    mpz_init(_ecc_a);
    mpz_init(_ecc_b);
    mpz_init(_ecc_Gx);
    mpz_init(_ecc_Gy);
    mpz_init(_ecc_n);
    mpz_init(_ecc_h);
    mpz_init(_ecc_d_key);
    mpz_init(_ecc_Qx_key);
    mpz_init(_ecc_Qy_key);
    mpz_init(_ecc_embed_k);
    
    _ecc_stand[0] = "secp256k1";
    _ecc_stand[1] = "sm2";
}

Crypt::~Crypt() {
    mpz_clear(_e_key);
    mpz_clear(_d_key);
    mpz_clear(_n_key);

    mpz_clear(_dl_p_key);
    mpz_clear(_dl_q_key);
    mpz_clear(_dl_g_key);
    mpz_clear(_dl_y_key);
    mpz_clear(_dl_x_key);

    mpz_clear(_ecc_p);
    mpz_clear(_ecc_a);
    mpz_clear(_ecc_b);
    mpz_clear(_ecc_Gx);
    mpz_clear(_ecc_Gy);
    mpz_clear(_ecc_n);
    mpz_clear(_ecc_h);
    mpz_clear(_ecc_d_key);
    mpz_clear(_ecc_Qx_key);
    mpz_clear(_ecc_Qy_key);
    mpz_clear(_ecc_embed_k);
}

void Crypt::Ecc_Init(string init_keys) {
    if (init_keys.compare(_ecc_stand[0]) == 0) {
        mpz_set_str(_ecc_p, "ffffffff00000001000000000000000000000000ffffffffffffffffffffffff", 16);
        mpz_set_str(_ecc_a, "ffffffff00000001000000000000000000000000fffffffffffffffffffffffc", 16);
        //mpz_set_str(_ecc_a, "-3", 10);
        mpz_set_str(_ecc_b, "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b", 16);
        mpz_set_str(_ecc_n, "ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551", 16);
        mpz_set_str(_ecc_Gx, "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296", 16);
        mpz_set_str(_ecc_Gy, "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5", 16);

    } else if (init_keys.compare(_ecc_stand[1]) == 0) {
        mpz_set_str(_ecc_p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF", 16);
        mpz_set_str(_ecc_a, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC", 16);
        //mpz_set_str(_ecc_a, "-3", 10);
        mpz_set_str(_ecc_b, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93", 16);
        mpz_set_str(_ecc_n, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123", 16);
        mpz_set_str(_ecc_Gx, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7", 16);
        mpz_set_str(_ecc_Gy, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0", 16);
    }
    mpz_set_ui(_ecc_embed_k, 30); 
    gmp_printf("_ecc_p: %Zx\n", _ecc_p);
    gmp_printf("_ecc_p: %Zd\n", _ecc_p);
    gmp_printf("_ecc_a: %Zx\n", _ecc_a);
    gmp_printf("_ecc_a: %Zd\n", _ecc_a);
    gmp_printf("_ecc_b: %Zx\n", _ecc_b);
    gmp_printf("_ecc_b: %Zd\n", _ecc_b);
    gmp_printf("_ecc_n: %Zx\n", _ecc_n);
    gmp_printf("_ecc_n: %Zd\n", _ecc_n);
    gmp_printf("_ecc_Gx: %Zx\n", _ecc_Gx);
    gmp_printf("_ecc_Gx: %Zd\n", _ecc_Gx);
    gmp_printf("_ecc_Gy: %Zx\n", _ecc_Gy);
    gmp_printf("_ecc_Gy: %Zd\n", _ecc_Gy);
    gmp_printf("_ecc_embed_k: %Zd\n", _ecc_embed_k);
}

string Crypt::_string_to_hex(string message) {
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

string Crypt::_hex_to_string(string _message) {
    string result = "";
    cout <<"string to hex size: " << _message.size() << " " << _message.length()<< endl;
    for (int i = 0; i < _message.size(); i += 2) {
        char chr = (char) (int) strtol(_message.substr(i, 2).c_str(), NULL, 16);
        cout << chr << endl;
        result.push_back(chr);
    }
    return result;
}

void Crypt::RSA_Plain_Generate_Keys(int p_length, int q_length, int is_out) {
    mpz_t _p ; mpz_init(_p);
    mpz_t _q ; mpz_init(_q);
    mpz_t _phi; mpz_init(_phi);
    mpz_t _p_1; mpz_init(_p_1);
    mpz_t _q_1; mpz_init(_q_1);

    mpz_urandomb(_p, _gstate, p_length) ;
    mpz_nextprime(_p, _p) ;

    mpz_urandomb(_q, _gstate, q_length) ;
    mpz_nextprime(_q, _q) ;
    mpz_mul(_n_key, _p, _q);
    mpz_sub_ui(_p_1, _p, 1); mpz_sub_ui(_q_1, _q, 1);

    mpz_mul(_phi, _p_1, _q_1);
    mpz_invert(_d_key, _e_key, _phi) ;

    if (is_out == 1) {
        gmp_printf("_p: %Zd\n", _p);
        gmp_printf("_q: %Zd\n", _q); 
        gmp_printf("_e_key: %Zd\n", _e_key);
        gmp_printf("_d_key: %Zd\n", _d_key);
        gmp_printf("_n_key: %Zd\n", _n_key);
    }
   
    mpz_clear(_p);
    mpz_clear(_q);
    mpz_clear(_phi);
    mpz_clear(_p_1);
    mpz_clear(_q_1);
}

void Crypt::RSA_Plain_Encryption(const char * message, mpz_t _C) {
    mpz_t _m; mpz_init(_m);
    mpz_set_str(_m, message, 10);
    mpz_powm(_C, _m, _e_key, _n_key) ;
    mpz_clear(_m);
    return ;
}

void Crypt::RSA_Plain_Decryption(mpz_t _C, mpz_t _M) {
    mpz_powm(_M, _C, _d_key, _n_key) ;
    return ;
}

void Crypt::RSA_Plain_Sign(const char * message, mpz_t signature, int is_out) {
    hash.MD5_init();
    string hash_tag = hash.MD5_value(string(message));
    mpz_t _hash_tag; mpz_init(_hash_tag) ;
    //mpz_set_str(_hash_tag, hash._hex2dec(hash_tag).c_str(), 10);
    mpz_set_str(_hash_tag, hash_tag.c_str(), 16);
    mpz_powm(signature, _hash_tag, _e_key, _n_key) ;
    if (is_out) {
        cout << "hash is: " << hash_tag << endl;
        cout << "dec hash is: " << hash._hex2dec(hash_tag) << endl;
        gmp_printf("signature: %Zd\n", signature) ;
        gmp_printf("_hash_tag: %Zx\n", _hash_tag) ;
    }
    mpz_clear(_hash_tag) ;
    return ;
}

int Crypt::RSA_Plain_Verify_Sign(const char * message, mpz_t signature, int is_out) {
    hash.MD5_init();
    string hash_tag = hash.MD5_value(string(message));
    mpz_t _hash_tag; mpz_init(_hash_tag) ;
    //mpz_set_str(_hash_tag, hash._hex2dec(hash_tag).c_str(), 10);
    mpz_set_str(_hash_tag, hash_tag.c_str(), 16);
    mpz_t _recov_hash_tag; mpz_init(_recov_hash_tag) ;
    mpz_powm(_recov_hash_tag, signature, _d_key, _n_key) ;
    if (is_out) {
        gmp_printf("_recovered hash: %Zx\n", _recov_hash_tag) ;
        gmp_printf("hash: %Zx\n", _hash_tag) ;
    }
    if (mpz_cmp(_recov_hash_tag, _hash_tag) == 0) {
        cout << "accept !" << endl;
        mpz_clear(_hash_tag); mpz_clear(_recov_hash_tag) ;
        return 1;
    }else {
        cout << "reject !" << endl;
        mpz_clear(_hash_tag); mpz_clear(_recov_hash_tag) ;
        return 0;
    }
}

void Crypt::DL_Generate_Keys(int q_length, int is_out) {
    mpz_t _temp_h; mpz_init(_temp_h);
    mpz_t _temp_p; mpz_init(_temp_p);
    mpz_t _temp_i; mpz_init(_temp_i);
    mpz_t _temp_q; mpz_init(_temp_q);

    mpz_urandomb(_dl_q_key, _gstate, q_length) ;
    mpz_nextprime(_dl_q_key, _dl_q_key) ;
    int i = 2;
    while (true) {
        mpz_mul_ui(_dl_p_key, _dl_q_key, i);
        mpz_add_ui(_dl_p_key, _dl_p_key, 1);
        if (mpz_probab_prime_p(_dl_p_key, prime_test_reps) != 0)  break;
        i += 1;
    }
    mpz_sub_ui(_temp_p, _dl_p_key, 1);
    mpz_add_ui(_temp_i, _temp_i, i);
    while (true) {
        // h \in [1, p-1]
        mpz_urandomm(_temp_h, _gstate, _temp_p);
        mpz_add_ui(_temp_h, _temp_h, 1);
        mpz_powm(_dl_g_key, _temp_h, _temp_i, _dl_p_key);
        if (mpz_cmp_ui(_dl_g_key, 1 ) != 0) break;
    }
    // x \in [1, q-1]
    mpz_sub_ui(_temp_q, _dl_q_key, 1);
    mpz_urandomm(_dl_x_key, _gstate, _temp_q);
    mpz_add_ui(_dl_x_key, _dl_x_key, 1);
    mpz_powm(_dl_y_key, _dl_g_key, _dl_x_key, _dl_p_key);
    if (is_out) {
        gmp_printf("p: %Zd\n", _dl_p_key);
        gmp_printf("q: %Zd\n", _dl_q_key);
        gmp_printf("g: %Zd\n", _dl_g_key);
        gmp_printf("y: %Zd\n", _dl_y_key);
        gmp_printf("x: %Zd\n", _dl_x_key);
    }

    mpz_clear(_temp_h);
    mpz_clear(_temp_p);
    mpz_clear(_temp_i);
    mpz_clear(_temp_q);
}

void Crypt::DL_ElGamal_Encryption(const char* message, mpz_t _C1, mpz_t _C2, int is_out) {
    // k \in [1, q-1] ; message \in [0, p-1] 
    mpz_t _temp_k; mpz_init(_temp_k);
    mpz_t _temp_q; mpz_init(_temp_q); 
    mpz_t _temp_m; mpz_init(_temp_m);
    mpz_sub_ui(_temp_q, _temp_q, 1);
    mpz_set_str(_temp_m, message, 10);
    mpz_urandomm(_temp_k, _gstate, _temp_q);
    mpz_add_ui(_temp_k, _temp_k, 1);
    // C1
    mpz_powm(_C1, _dl_g_key, _temp_k, _dl_p_key); 
    // C2
    mpz_powm(_C2, _dl_y_key, _temp_k, _dl_p_key);
    mpz_mod(_temp_m, _temp_m, _dl_p_key);
    mpz_mul(_C2, _temp_m, _C2);
    mpz_mod(_C2, _C2, _dl_p_key);
    if (is_out) {
        gmp_printf("_C1: %Zd\n", _C1);
        gmp_printf("_C2: %Zd\n", _C2);
    }
    // clear 
    mpz_clear(_temp_k);
    mpz_clear(_temp_q);
    mpz_clear(_temp_m);
}

void Crypt::DL_ElGamal_Decryption(mpz_t _C1, mpz_t _C2, mpz_t _M, int is_out) {
    mpz_t _inverse; mpz_init(_inverse);
    mpz_powm(_inverse, _C1, _dl_x_key, _dl_p_key);
    mpz_invert(_inverse, _inverse, _dl_p_key);
    mpz_mul(_inverse, _C2, _inverse);
    mpz_mod(_M, _inverse, _dl_p_key);
    mpz_clear(_inverse);
    if (is_out) {
        gmp_printf("_M: %Zd\n", _M);
    }
}

void Crypt::DL_ElGamal_Sign(const char* message, mpz_t _R, mpz_t _S, int is_out) {
    hash.MD5_init();
    string hash_tag = hash.MD5_value(string(message));
    mpz_t _hash_tag; mpz_init(_hash_tag) ;
    mpz_set_str(_hash_tag, hash_tag.c_str(), 16);
    // k \in [1, q-1]
    mpz_t _temp_k; mpz_init(_temp_k);
    mpz_t _temp_q; mpz_init(_temp_q);
    mpz_t _T; mpz_init(_T);
    mpz_t _k_inverse; mpz_init(_k_inverse);
    mpz_t _xr; mpz_init(_xr);
    mpz_t _temp_1; mpz_init(_temp_1);
    mpz_sub_ui(_temp_q, _dl_q_key, 1);
    cout << "1" << endl;
    while (true) {
        mpz_urandomm(_temp_k, _gstate, _temp_q);
        mpz_add_ui(_temp_k, _temp_k, 1);
        // T = g ^ k mod p
        mpz_powm(_T, _dl_g_key, _temp_k, _dl_p_key); 
        mpz_mod(_R, _T, _dl_q_key);
        if (mpz_cmp_ui(_R, 0) == 0) continue;
        // s = k ^ -1 (h + xr) mod q 
        mpz_invert(_k_inverse , _temp_k, _dl_q_key);
        mpz_mul(_xr, _dl_x_key, _R);
        mpz_add(_xr, _hash_tag, _xr);
        mpz_mod(_temp_1, _xr, _dl_q_key);
        mpz_mul(_temp_1, _k_inverse, _temp_1);
        mpz_mod(_S, _temp_1, _dl_q_key);
        if (mpz_cmp_ui(_S, 0) == 0) continue;
        else break;
    }
    mpz_clear(_hash_tag);
    mpz_clear(_temp_k);
    mpz_clear(_temp_q);
    mpz_clear(_T);
    mpz_clear(_k_inverse);
    mpz_clear(_xr);
    mpz_clear(_temp_1);
}

int Crypt::DL_ElGamal_Verify_Sign(const char* message, mpz_t _R, mpz_t _S, int is_out) {
    if ( (mpz_cmp_ui(_R, 1) < 0 || mpz_cmp(_R, _dl_q_key)>=0 ) || (mpz_cmp_ui(_S, 1) < 0 || mpz_cmp(_S, _dl_q_key)>=0) ) {
        cout << "reject !" << endl;
        return 0;
    }
    hash.MD5_init();
    string hash_tag = hash.MD5_value(string(message));
    mpz_t _hash_tag; mpz_init(_hash_tag) ;
    mpz_set_str(_hash_tag, hash_tag.c_str(), 16);
    mpz_t _W; mpz_init(_W);
    mpz_t _u1_; mpz_init(_u1_);
    mpz_t _u2_; mpz_init(_u2_);
    mpz_t _u1; mpz_init(_u1);
    mpz_t _u2; mpz_init(_u2);
    mpz_t _T; mpz_init(_T);
    mpz_t _R_Prime; mpz_init(_R_Prime);

    mpz_invert(_W, _S, _dl_q_key);
    mpz_mul(_u1_, _hash_tag, _W); 
    mpz_mod(_u1, _u1_, _dl_q_key);
    mpz_mul(_u2_, _R, _W);
    mpz_mod(_u2, _u2_, _dl_q_key);
    // T = g^u1 y^u2 mod p
    mpz_powm(_u1_, _dl_g_key, _u1, _dl_p_key);
    mpz_powm(_u2_, _dl_y_key, _u2, _dl_p_key);
    mpz_mul(_T, _u1_, _u2_);
    mpz_mod(_T, _T, _dl_p_key);
    mpz_mod(_R_Prime, _T, _dl_q_key);
    if (is_out) {
        gmp_printf("_hash: %Zx\n", _hash_tag);
        gmp_printf("_R: %Zd\n", _R);
        gmp_printf("_R_Prime: %Zd\n", _R_Prime);
    }
    if (mpz_cmp(_R, _R_Prime) == 0) {
        cout << "Acc !" << endl;
        mpz_clear(_hash_tag);
        mpz_clear(_W);
        mpz_clear(_u1_);
        mpz_clear(_u2_);
        mpz_clear(_u1);
        mpz_clear(_u2);
        mpz_clear(_T);
        mpz_clear(_R_Prime);
        return 1;
    }else{
        cout << "Reject !" << endl;
        mpz_clear(_hash_tag);
        mpz_clear(_W);
        mpz_clear(_u1_);
        mpz_clear(_u2_);
        mpz_clear(_u1);
        mpz_clear(_u2);
        mpz_clear(_T);
        mpz_clear(_R_Prime);
        return 0;
    }
}

// three = one + two
void Crypt::_ecc_point_addition(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y, mpz_t three_x, mpz_t three_y) {
    mpz_t _temp1; mpz_init(_temp1);
    mpz_t _temp2; mpz_init(_temp2);
    mpz_t _temp2_; mpz_init(_temp2_);
    mpz_t _temp3; mpz_init(_temp3);
    mpz_t _slop; mpz_init(_slop);

    if ((mpz_cmp_ui(one_x,0) == 0 && mpz_cmp_ui(one_y, 0) == 0) && (mpz_cmp_ui(two_x,0) == 0 && mpz_cmp_ui(two_y, 0) == 0 )) {
        mpz_set_ui(three_x, 0);
        mpz_set_ui(three_y, 0);
        return ;
    }
    if ((mpz_cmp_ui(one_x,0) == 0 && mpz_cmp_ui(one_y, 0) == 0) ) {
        mpz_set(three_x, two_x);
        mpz_set(three_y, two_y);
        return ;
    }

    if ((mpz_cmp_ui(two_x,0) == 0 && mpz_cmp_ui(two_y, 0) == 0) ) {
        mpz_set(three_x, one_x);
        mpz_set(three_y, one_y);
        return ;
    }
    
    if (mpz_cmp_ui(two_y, 0) != 0) {
        mpz_sub(_temp1, _ecc_p, two_y);
        mpz_mod(_temp1, _temp1, _ecc_p);
    }else 
        mpz_set_ui(_temp1, 0);
    if (mpz_cmp(one_y, _temp1) == 0 && mpz_cmp(one_x, two_x) == 0) {
        mpz_set_ui(three_x, 0);
        mpz_set_ui(three_y, 0);
        return ;
    } 
    if (mpz_cmp(one_x, two_x) == 0 && mpz_cmp(one_y, two_y) == 0) {
        _ecc_point_doubling(one_x, one_y, three_x, three_y);
        return ;
    }
    mpz_sub(_temp1, one_y, two_y);
    mpz_sub(_temp2, one_x, two_x);
    mpz_mod(_temp2, _temp2, _ecc_p);
    mpz_invert(_temp2_, _temp2, _ecc_p);
    mpz_mul(_slop, _temp1, _temp2_);
    mpz_mod(_slop, _slop, _ecc_p); // slop mod p

    mpz_mul(_temp3, _slop, _slop);
    mpz_sub(_temp3, _temp3, one_x);
    mpz_sub(three_x, _temp3, two_x); // three_x 
    mpz_mod(three_x, three_x, _ecc_p); // three_x mod p 

    mpz_sub(_temp2 , one_x, three_x);
    mpz_mul(_temp3, _slop, _temp2);
    mpz_sub(three_y, _temp3, one_y); // three_y
    mpz_mod(three_y, three_y, _ecc_p);

    mpz_clear(_temp1);
    mpz_clear(_temp2);
    mpz_clear(_temp2_);
    mpz_clear(_temp3);
    mpz_clear(_slop);

    return ;
}

void Crypt::_ecc_point_doubling(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y) {
    if (mpz_cmp_ui(one_y, 0) == 0 ) { 
        mpz_set_ui(two_x, 0);
        mpz_set_ui(two_y, 0);
        return ;
    }
    mpz_t _slop; mpz_init(_slop);
    mpz_t _temp1; mpz_init(_temp1);
    mpz_t _temp2; mpz_init(_temp2);
    mpz_t _temp2_; mpz_init(_temp2_);
    mpz_t _temp3; mpz_init(_temp3);

    mpz_pow_ui(_temp1, one_x, 2);
    mpz_mul_ui(_temp1, _temp1, 3);
    mpz_add(_temp1, _temp1, _ecc_a);

    mpz_mul_ui(_temp2, one_y, 2);
    mpz_invert(_temp2_, _temp2, _ecc_p);
    mpz_mul(_slop, _temp1, _temp2_);
    mpz_mod(_slop, _slop, _ecc_p); // slop mod p

    mpz_pow_ui(_temp1, _slop, 2);
    mpz_mul_ui(_temp2, one_x, 2);
    mpz_sub(_temp3, _temp1, _temp2);
    mpz_mod(_temp3, _temp3, _ecc_p); // two_x mode p

    mpz_sub(_temp1, one_x, _temp3);
    mpz_mul(_temp2, _slop, _temp1);
    mpz_sub(two_y, _temp2, one_y);
    mpz_mod(two_y, two_y, _ecc_p); // two_y mode p
    mpz_set(two_x, _temp3);

    mpz_clear(_temp1);
    mpz_clear(_temp2);
    mpz_clear(_temp2_);
    mpz_clear(_temp3);
    mpz_clear(_slop);

    return ;
}

void Crypt::_ecc_point_doubling(mpz_t one_x, mpz_t one_y) {
    //if (mpz_cmp_ui(one_y, 0) == 0 && mpz_cmp_ui(one_x, 0) == 0 ) return;
    if (mpz_cmp_ui(one_y, 0) == 0 ) { 
        mpz_set_ui(one_x, 0);
        return ;
    }
    mpz_t _slop; mpz_init(_slop);
    mpz_t _temp1; mpz_init(_temp1);
    mpz_t _temp2; mpz_init(_temp2);
    mpz_t _temp2_; mpz_init(_temp2_);
    mpz_t _temp3; mpz_init(_temp3);

    mpz_pow_ui(_temp1, one_x, 2);
    mpz_mul_ui(_temp1, _temp1, 3);
    mpz_add(_temp1, _temp1, _ecc_a);

    mpz_mul_ui(_temp2, one_y, 2);
    mpz_invert(_temp2_, _temp2, _ecc_p);
    mpz_mul(_slop, _temp1, _temp2_);
    mpz_mod(_slop, _slop, _ecc_p); // slop mod p

    mpz_pow_ui(_temp1, _slop, 2);
    mpz_mul_ui(_temp2, one_x, 2);
    mpz_sub(_temp3, _temp1, _temp2);
    mpz_mod(_temp3, _temp3, _ecc_p); // two_x mode p

    mpz_sub(_temp1, one_x, _temp3);
    mpz_mul(_temp2, _slop, _temp1);
    mpz_sub(_temp1, _temp2, one_y);
    mpz_mod(one_y, _temp1, _ecc_p); // two_y mode p
    mpz_set(one_x, _temp3);

    mpz_clear(_temp1);
    mpz_clear(_temp2);
    mpz_clear(_temp2_);
    mpz_clear(_temp3);
    mpz_clear(_slop);

    return ;
}


// three = one * two
void Crypt::_ecc_point_multiplication(mpz_t _K, mpz_t _ecc_t1, mpz_t _ecc_t2, mpz_t Q_x, mpz_t Q_y) {
    mpz_set_ui(Q_x, 0); mpz_set_ui(Q_y, 0);
    if (mpz_cmp_ui(_K, 0) == 0) return;

    mpz_t _temp_k; mpz_init(_temp_k);
    mpz_set(_temp_k, _K);
    int * _A = _ecc_NAF(_temp_k);
    int lens = _A[0];

    mpz_t _temp_x; mpz_init(_temp_x);
    mpz_t _temp_y; mpz_init(_temp_y);
    mpz_t _temp_ecc_t2; mpz_init(_temp_ecc_t2);
    mpz_mul_si(_temp_ecc_t2, _ecc_t2, -1);
    mpz_mod(_temp_ecc_t2, _temp_ecc_t2, _ecc_p);
    for (int i = lens-1 ; i >= 1; i-- ){
        _ecc_point_doubling(Q_x, Q_y);
        if (_A[i] == 1)  {
                _ecc_point_addition(Q_x, Q_y, _ecc_t1, _ecc_t2, _temp_x, _temp_y);
                mpz_set(Q_x, _temp_x);
                mpz_set(Q_y, _temp_y);
        }else if (_A[i] == -1) {
                _ecc_point_addition(Q_x, Q_y, _ecc_t1, _temp_ecc_t2, _temp_x, _temp_y);
                mpz_set(Q_x, _temp_x);
                mpz_set(Q_y, _temp_y);
        }
    } 
    mpz_clear(_temp_k);
    mpz_clear(_temp_x);
    mpz_clear(_temp_y);
    mpz_clear(_temp_ecc_t2);
    delete _A;
}

int * Crypt::_ecc_NAF(mpz_t _K) {
    int i = 1; 
    int * _naf = new int[MAX_ARRAY_LEN];
    mpz_t _temp; mpz_init(_temp);
    unsigned long int _quot;
    while(mpz_cmp_ui(_K, 1) >= 0) {
        if (mpz_odd_p(_K) != 0) {
            mpz_mod_ui(_temp, _K, 4);
            _naf[i] = 2 - (int)mpz_get_si(_temp) ;
            if (_naf[i] > 0)
                mpz_sub_ui(_K, _K, _naf[i]) ;
            else if (_naf[i] < 0) 
                mpz_add_ui(_K, _K, -1*_naf[i]);
        }else {
            _naf[i] = 0;
        }
        _quot = mpz_fdiv_q_ui(_K, _K, 2) ;
        i += 1;
    }
    _naf[0] = i;
    mpz_clear(_temp);
    return _naf;
}

void Crypt::_ecc_plain_text_embeded(string message, mpz_t _mess_x, mpz_t _mess_y) {
    // uncomplished, the case when _message > _ecc_p 
    mpz_t _message; mpz_init(_message);
    mpz_set_str(_message, _string_to_hex(message).c_str(), 16);
    gmp_printf("message: %Zx\n", _message);
    gmp_printf("message: %Zd\n", _message);
    mpz_t _temp_m1 ; mpz_init(_temp_m1);
    mpz_set(_temp_m1, _message);
    mpz_add_ui(_temp_m1, _temp_m1, 1);
    mpz_mul(_temp_m1, _temp_m1, _ecc_embed_k);
    if (mpz_cmp(_temp_m1, _ecc_p) >= 0) {
        cout <<"text too large .. exit"<<endl;
        exit(0);
    }
    mpz_mul(_mess_x, _ecc_embed_k, _message);
    gmp_printf("_mess_x: %Zd\n", _mess_x);
    mpz_t _temp_m2; mpz_init(_temp_m2);
    mpz_t _temp_m3; mpz_init(_temp_m3);

    mpz_t _remd1; mpz_init(_remd1);
    mpz_t _remd2; mpz_init(_remd2);
    
    mpz_sub_ui(_remd1, _ecc_p, 1);
    mpz_fdiv_q_ui(_remd1, _remd1, 2);
    
    mpz_add_ui(_remd2, _ecc_p, 1);
    mpz_fdiv_q_ui(_remd2, _remd2, 4);
    
    int i = 0;
    while(mpz_cmp_ui(_ecc_embed_k, i) >= 0) {
        mpz_pow_ui(_temp_m2, _mess_x, 3);
        mpz_mul(_temp_m3, _mess_x, _ecc_a);
        mpz_add(_temp_m2, _temp_m2, _temp_m3);
        mpz_add(_temp_m2, _temp_m2, _ecc_b);
        mpz_powm(_temp_m3, _temp_m2, _remd1, _ecc_p);
        if (mpz_cmp_ui(_temp_m3, 0) > 0) {
            mpz_powm(_mess_y, _temp_m2, _remd2, _ecc_p);
            break;
        }
        i += 1;
        mpz_add_ui(_mess_x, _mess_x, 1);
    }
    mpz_clear(_message);
    mpz_clear(_temp_m1);
    mpz_clear(_temp_m2);
    mpz_clear(_temp_m3);
    mpz_clear(_remd1);
    mpz_clear(_remd2);
}

void Crypt::_ecc_cyber_text_decode(mpz_t _mess_x, mpz_t _mess_y, mpz_t _message) {
    mpz_fdiv_q(_message, _mess_x, _ecc_embed_k);
}

void Crypt::ECC_Generate_Keys(string init_keys) {
    Ecc_Init(init_keys);

    mpz_t _temp_n; mpz_init(_temp_n);
    mpz_sub_ui(_temp_n, _ecc_n, 1);
    mpz_urandomm(_ecc_d_key, _gstate, _temp_n);
    mpz_add_ui(_ecc_d_key, _ecc_d_key, 1);
    
    _ecc_point_multiplication(_ecc_d_key, _ecc_Gx, _ecc_Gy, _ecc_Qx_key, _ecc_Qy_key);

    gmp_printf("_ecc_d_key 2: %Zd\n", _ecc_d_key);
    gmp_printf("_ecc_Gx 2: %Zd\n", _ecc_Gx);
    gmp_printf("_ecc_Gy 2: %Zd\n", _ecc_Gy);
    gmp_printf("_ecc_Qx_key 2: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 2: %Zd\n", _ecc_Qy_key); 

    mpz_clear(_temp_n);
}

void Crypt::ECC_Encryption(string message, mpz_t C1_x, mpz_t C1_y, mpz_t C2_x, mpz_t C2_y) {
    mpz_t _mess_x; mpz_init(_mess_x);
    mpz_t _mess_y; mpz_init(_mess_y);
    _ecc_plain_text_embeded(message, _mess_x, _mess_y);
    gmp_printf("_mess_x : %Zd\n", _mess_x);
    gmp_printf("_mess_y : %Zd\n", _mess_y);

    mpz_t _temp_n; mpz_init(_temp_n);
    mpz_t _temp_k; mpz_init(_temp_k);
    mpz_sub_ui(_temp_n, _ecc_n, 1);
    mpz_urandomm(_temp_k, _gstate, _temp_n);
    mpz_add_ui(_temp_k, _temp_k, 1);

    mpz_t _temp_c2_x; mpz_init(_temp_c2_x); 
    mpz_t _temp_c2_y; mpz_init(_temp_c2_y); 
    

    _ecc_point_multiplication(_temp_k, _ecc_Gx, _ecc_Gy, C1_x, C1_y); // c1 = kp
    gmp_printf("_ecc_Qx_key 2: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 2: %Zd\n", _ecc_Qy_key);
    gmp_printf("_temp_k 2: %Zd\n", _temp_k);

    _ecc_point_multiplication(_temp_k, _ecc_Qx_key, _ecc_Qy_key, _temp_c2_x, _temp_c2_y); // c2 = M + kp
    gmp_printf("_ecc_Qx_key 3: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 3: %Zd\n", _ecc_Qy_key);
    gmp_printf("_temp_k 3: %Zd\n", _temp_k);

    gmp_printf("_temp_c2_x : %Zd\n", _temp_c2_x);
    gmp_printf("_temp_c2_y : %Zd\n", _temp_c2_y);
    _ecc_point_addition(_mess_x, _mess_y, _temp_c2_x, _temp_c2_y, C2_x, C2_y); 
    gmp_printf("_mess_x 2: %Zd\n", _mess_x);
    gmp_printf("_mess_y 2: %Zd\n", _mess_y);

    mpz_t dk; mpz_init(dk);
    mpz_t dk_x; mpz_init(dk_x);
    mpz_t dk_y; mpz_init(dk_y);
    mpz_t dk_x2; mpz_init(dk_x2);
    mpz_t dk_y2; mpz_init(dk_y2);
    mpz_t dk_x3; mpz_init(dk_x3);
    mpz_t dk_y3; mpz_init(dk_y3);

    mpz_mul(dk, _temp_k, _ecc_d_key);
    gmp_printf("dk: %Zd\n", dk);
    _ecc_point_multiplication(_temp_k, _ecc_Gx, _ecc_Gy, dk_x, dk_y);
    gmp_printf("dk_x 1: %Zd\n", dk_x);
    gmp_printf("dk_y 1: %Zd\n", dk_y);
    //mpz_set_ui(dk_x, 0); mpz_set_ui(dk_y, 0);
    _ecc_point_multiplication(_ecc_d_key, dk_x, dk_y, dk_x2, dk_y2);
    gmp_printf("dk_x 2: %Zd\n", dk_x2);
    gmp_printf("dk_y 2: %Zd\n", dk_y2);
    gmp_printf("dk 2: %Zd\n", dk);

    mpz_set_ui(dk_x, 0); mpz_set_ui(dk_y, 0);
    mpz_set_ui(dk_x2, 0); mpz_set_ui(dk_y2, 0);
    _ecc_point_multiplication(_ecc_d_key, _ecc_Gx, _ecc_Gy, dk_x, dk_y);
    gmp_printf("dk_x 3: %Zd\n", dk_x);
    gmp_printf("dk_y 3: %Zd\n", dk_y);
    _ecc_point_multiplication(_temp_k, dk_x, dk_y, dk_x2, dk_y2);
    gmp_printf("dk_x 4: %Zd\n", dk_x2);
    gmp_printf("dk_y 4: %Zd\n", dk_y2);

    gmp_printf("_mess_x 3: %Zd\n", _mess_x);
    gmp_printf("_mess_y 3: %Zd\n", _mess_y);

    _ecc_point_addition(_mess_x, _mess_y, dk_x2, dk_y2, dk_x3, dk_y3);
    gmp_printf("dk_x 5: %Zd\n", dk_x3);
    gmp_printf("dk_y 5: %Zd\n", dk_y3);

    mpz_t dk_y4; mpz_init(dk_y4);
    mpz_t dk_x5; mpz_init(dk_x5);
    mpz_t dk_y5; mpz_init(dk_y5);
    mpz_sub(dk_y4, _ecc_p, dk_y2);
    mpz_mod(dk_y4, dk_y4, _ecc_p);
    gmp_printf("dk_y4 : %Zd\n", dk_y4);
    _ecc_point_addition(dk_x3, dk_y3, dk_x2, dk_y4, dk_x5, dk_y5);
    gmp_printf("dk_x 6: %Zd\n", dk_x5);
    gmp_printf("dk_y 6: %Zd\n", dk_y5);

    mpz_sub(dk_y4, _ecc_p, _mess_y);
    mpz_mod(dk_y4, dk_y4, _ecc_p);
    gmp_printf("dk_y4 : %Zd\n", dk_y4);
    mpz_set_ui(dk_x5, 0); mpz_set_ui(dk_y5, 0);
    _ecc_point_addition(dk_x3, dk_y3, _mess_x, dk_y4, dk_x5, dk_y5);
    gmp_printf("dk_x 7: %Zd\n", dk_x5);
    gmp_printf("dk_y 7: %Zd\n", dk_y5);

    mpz_clear(_mess_x);
    mpz_clear(_mess_y);
    mpz_clear(_temp_n);
    mpz_clear(_temp_k);
    mpz_clear(_temp_c2_x);
    mpz_clear(_temp_c2_y);
}

void Crypt::ECC_Decryption(mpz_t C1_x, mpz_t C1_y, mpz_t C2_x, mpz_t C2_y) {
    mpz_t _temp_x; mpz_init(_temp_x);
    mpz_t _temp_y; mpz_init(_temp_y);
    mpz_t _temp_y2; mpz_init(_temp_y2);
    mpz_t mess_x; mpz_init(mess_x);
    mpz_t mess_y; mpz_init(mess_y);
    mpz_t _message; mpz_init(_message);
    
    _ecc_point_multiplication(_ecc_d_key, C1_x, C1_y, _temp_x, _temp_y);

    gmp_printf("_temp x: %Zd\n", _temp_x);
    gmp_printf("_temp y: %Zd\n", _temp_y);
    //mpz_mul_si(_temp_y, _temp_y, -1);
    mpz_sub(_temp_y2, _ecc_p, _temp_y);
    mpz_mod(_temp_y2, _temp_y2, _ecc_p);
    gmp_printf("_temp y2: %Zd\n", _temp_y2);
    _ecc_point_addition(C2_x, C2_y, _temp_x, _temp_y2, mess_x, mess_y);

    gmp_printf("decrypted mess x: %Zd\n", mess_x);
    gmp_printf("decrypted mess y: %Zd\n", mess_y);
    mpz_fdiv_q(_message, mess_x, _ecc_embed_k);
    gmp_printf("_message: %Zd\n", _message);
    char *_str = new char[MAX_ARRAY_LEN];
    mpz_get_str(_str, 16, _message);
    cout << _str << endl;
    string de_mess, result; de_mess = _str;
    result = _hex_to_string(de_mess);
    cout << "decrypted message: " << result << endl;
    mpz_clear(_temp_x);
    mpz_clear(_temp_y);
    mpz_clear(mess_x);
    mpz_clear(mess_y);
    mpz_clear(_message);
    delete _str;
}

void Crypt::ECC_Sign(string message, mpz_t _r, mpz_t _s) {
    mpz_t _k; mpz_init(_k);
    mpz_t _e; mpz_init(_e);
    mpz_t _hash_tag; mpz_init(_hash_tag) ;
    mpz_t _temp_x; mpz_init(_temp_x);
    mpz_t _temp_y; mpz_init(_temp_y);
    mpz_t _temp_n; mpz_init(_temp_n);
    mpz_sub_ui(_temp_n, _ecc_n, 1);
    string hash_tag;
    hash.MD5_init();
    //while ( mpz_cmp_ui(_r, 0) == 0) {
    while (true) {
        mpz_urandomm(_k, _gstate, _temp_n);
        mpz_add_ui(_k, _k, 1);
        _ecc_point_multiplication(_k, _ecc_Gx, _ecc_Gy, _temp_x, _temp_y);
        mpz_mod(_r, _temp_x, _ecc_n);
        if (mpz_cmp_ui(_r, 0) == 0) continue;
        hash_tag = hash.MD5_value(message);
        mpz_set_str(_e, hash_tag.c_str(), 16);
        mpz_invert(_s, _k, _ecc_n);
        mpz_mul(_temp_y, _ecc_d_key, _r);
        mpz_add(_temp_y, _e, _temp_y);
        mpz_mul(_s, _s, _temp_y);
        mpz_mod(_s, _s, _ecc_n);
        if (mpz_cmp_ui(_s, 0) == 0) continue;
        else break;
    }
    mpz_clear(_k);
    mpz_clear(_e);
    mpz_clear(_hash_tag);
    mpz_clear(_temp_x);
    mpz_clear(_temp_y);
    mpz_clear(_temp_n);
    return;
}

int Crypt::ECC_Verify_Sign(string message, mpz_t _r, mpz_t _s) {
    if ( !(mpz_cmp_ui(_r, 1) >=0 && mpz_cmp(_r, _ecc_n) < 0 && mpz_cmp_ui(_s, 1) >=0 && mpz_cmp(_s, _ecc_n) < 0 )) return 0; 

    mpz_t _e; mpz_init(_e);
    mpz_t _w; mpz_init(_w);
    mpz_t _u1; mpz_init(_u1);
    mpz_t _u2; mpz_init(_u2);
    mpz_t _temp_x1; mpz_init(_temp_x1);
    mpz_t _temp_y1; mpz_init(_temp_y1);
    mpz_t _temp_x2; mpz_init(_temp_x2);
    mpz_t _temp_y2; mpz_init(_temp_y2);
    mpz_t _X_x; mpz_init(_X_x);
    mpz_t _X_y; mpz_init(_X_y);
    mpz_t _v; mpz_init(_v);

    hash.MD5_init();
    string hash_tag = hash.MD5_value(message);
    mpz_set_str(_e, hash_tag.c_str(), 16);
    mpz_invert(_w, _s, _ecc_n);
    mpz_mod(_w, _w, _ecc_n);
    mpz_mul(_u1, _e, _w);
    mpz_mod(_u1, _u1, _ecc_n);
    mpz_mul(_u2, _r, _w);
    mpz_mod(_u2, _u2, _ecc_n);
    _ecc_point_multiplication(_u1, _ecc_Gx, _ecc_Gy, _temp_x1, _temp_y1);
    _ecc_point_multiplication(_u2, _ecc_Qx_key, _ecc_Qy_key, _temp_x2, _temp_y2);
    _ecc_point_addition(_temp_x1, _temp_y1, _temp_x2, _temp_y2, _X_x, _X_y);
    if (mpz_cmp_ui(_X_x, 0) == 0 && mpz_cmp_ui(_X_y, 0) == 0) return 0;
    mpz_mod(_v, _X_x, _ecc_n);
    gmp_printf("_v: %Zd\n", _v);
    int res = -1;
    if (mpz_cmp(_v, _r) == 0) res = 1;
    else res = 0;
    
    mpz_clear(_e);
    mpz_clear(_w);
    mpz_clear(_u1);
    mpz_clear(_u2);
    mpz_clear(_temp_x1);
    mpz_clear(_temp_y1);
    mpz_clear(_temp_x2);
    mpz_clear(_temp_y2);
    mpz_clear(_X_x);
    mpz_clear(_X_y);
    mpz_clear(_v);
    
    return res; 
}
