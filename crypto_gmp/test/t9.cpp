#include "t9.h"

Crypt::Crypt() {
    prime_test_reps = 40;

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
        //mpz_set_str(_ecc_a, "ffffffff00000001000000000000000000000000fffffffffffffffffffffffc", 16);
        mpz_set_str(_ecc_a, "-3", 10);
        mpz_set_str(_ecc_b, "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b", 16);
        mpz_set_str(_ecc_n, "ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551", 16);
        mpz_set_str(_ecc_Gx, "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296", 16);
        mpz_set_str(_ecc_Gy, "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5", 16);
        
    } else if (init_keys.compare(_ecc_stand[1]) == 0) {
        mpz_set_str(_ecc_p, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF", 16);
        //mpz_set_str(_ecc_a, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC", 16);
        mpz_set_str(_ecc_a, "-3", 10);
        mpz_set_str(_ecc_b, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93", 16);
        mpz_set_str(_ecc_n, "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123", 16);
        mpz_set_str(_ecc_Gx, "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7", 16);
        mpz_set_str(_ecc_Gy, "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0", 16);

    }  else{
        mpz_set_str(_ecc_p, "23", 10);
        mpz_set_str(_ecc_a, "1", 10);
        mpz_set_str(_ecc_b, "1", 10);
        mpz_set_str(_ecc_n, "17", 10);
        mpz_set_str(_ecc_Gx, "0", 10);
        mpz_set_str(_ecc_Gy, "2", 10);
   
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
        char chr = (char) (int) strtol(result.substr(i, 2).c_str(), NULL, 16);
        result.push_back(chr);
    }
    return result;
}

// three = one + two
int Crypt::_ecc_point_addition(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y, mpz_t three_x, mpz_t three_y) {
    mpz_t _temp1; mpz_init(_temp1);
    mpz_t _temp2; mpz_init(_temp2);
    mpz_t _temp2_; mpz_init(_temp2_);
    mpz_t _temp3; mpz_init(_temp3);
    mpz_t _slop; mpz_init(_slop);
    
    if ((mpz_cmp_ui(one_x,0) == 0 && mpz_cmp_ui(one_y, 0) == 0) && (mpz_cmp_ui(two_x,0) == 0 && mpz_cmp_ui(two_y, 0) == 0 )) {
        mpz_set_ui(three_x, 0);
        mpz_set_ui(three_y, 0);
        return 1;
    }
    if ((mpz_cmp_ui(one_x,0) == 0 && mpz_cmp_ui(one_y, 0) == 0) ) {
        mpz_set(three_x, two_x);
        mpz_set(three_y, two_y);
        return 0;
    }

    if ((mpz_cmp_ui(two_x,0) == 0 && mpz_cmp_ui(two_y, 0) == 0) ) {
        mpz_set(three_x, one_x);
        mpz_set(three_y, one_y);
        return 0;
    }
    if (mpz_cmp_ui(two_y, 0) != 0) {
        mpz_sub(_temp1, _ecc_p, two_y);
        mpz_mod(_temp1, _temp1, _ecc_p);
    }else 
        mpz_set_ui(_temp1, 0);
    if (mpz_cmp(one_y, _temp1) == 0 && mpz_cmp(one_x, two_x) == 0) {
        mpz_set_ui(three_x, 0);
        mpz_set_ui(three_y, 0);
        return 1;
    } 
    if (mpz_cmp(one_x, two_x) == 0 && mpz_cmp(one_y, two_y) == 0) {
        _ecc_point_doubling(one_x, one_y, three_x, three_y);
        return 1;
    }
    /*
    if (mpz_cmp(one_x, two_x) == 0) { 
        mpz_t _two_y; mpz_init(_two_y);
        mpz_mul_si(_two_y, two_y, -1);

        mpz_mod(_two_y, _two_y, _ecc_p);
        gmp_printf("_two _y : %Zd\n", _two_y);
        if (mpz_cmp(one_y, _two_y) == 0) {
            cout << " infinity " << endl;
            mpz_set_ui(three_x, 0);
            mpz_set_ui(three_y, 0);
            mpz_clear(_two_y);
            return 1;
        }
        mpz_clear(_two_y);
    } */

    mpz_sub(_temp1, one_y, two_y);
    gmp_printf("_temp1 1: %Zd\n", _temp1);
    mpz_sub(_temp2, one_x, two_x);
    gmp_printf("_temp2 2: %Zd\n", _temp2);
    mpz_invert(_temp2_, _temp2, _ecc_p);
    gmp_printf("_temp2_ 3: %Zd\n", _temp2_);
    mpz_mul(_slop, _temp1, _temp2_);
    gmp_printf("_slop 4: %Zd\n", _slop);
    //mpz_fdiv_q(_slop, _temp1, _temp2); // slop
    mpz_mod(_slop, _slop, _ecc_p); // slop mod p
    gmp_printf("_slop 5: %Zd\n", _slop);

    mpz_mul(_temp3, _slop, _slop);
    gmp_printf("_temp3 6: %Zd\n", _temp3);
    mpz_sub(_temp3, _temp3, one_x);
    gmp_printf("_temp3 7: %Zd\n", _temp3);
    mpz_sub(three_x, _temp3, two_x); // three_x 
    gmp_printf("_three_x 8: %Zd\n", three_x);
    mpz_mod(three_x, three_x, _ecc_p); // three_x mod p 
    gmp_printf("_three_x 9: %Zd\n", three_x);

    mpz_sub(_temp2 , one_x, three_x);
    gmp_printf("_temp2 10: %Zd\n", _temp2);
    mpz_mul(_temp3, _slop, _temp2);
    gmp_printf("_temp3 10: %Zd\n", _temp3);
    mpz_sub(three_y, _temp3, one_y); // three_y
    gmp_printf("_three_y 11: %Zd\n", three_y);
    mpz_mod(three_y, three_y, _ecc_p);
    gmp_printf("_three_y 12: %Zd\n", three_y);

    mpz_clear(_temp1);
    mpz_clear(_temp2);
    mpz_clear(_temp2_);
    mpz_clear(_temp3);
    mpz_clear(_slop);
    return 0;
}

int Crypt::_ecc_point_doubling(mpz_t one_x, mpz_t one_y, mpz_t two_x, mpz_t two_y) {
    //if (mpz_cmp_ui(one_y, 0) == 0 && mpz_cmp_ui(one_x, 0) == 0 ) return;
    if (mpz_cmp_ui(one_y, 0) == 0 ) { 
        mpz_set_ui(two_x, 0);
        mpz_set_ui(two_y, 0);
        return 1;
    }
    mpz_t _slop; mpz_init(_slop);
    mpz_t _temp1; mpz_init(_temp1);
    mpz_t _temp2; mpz_init(_temp2);
    mpz_t _temp3; mpz_init(_temp3);

    mpz_pow_ui(_temp1, one_x, 2);
    mpz_mul_ui(_temp1, _temp1, 3);
    mpz_add(_temp1, _temp1, _ecc_a);
    mpz_mul_ui(_temp2, one_y, 2);
    mpz_fdiv_q(_slop, _temp1, _temp2);
    mpz_mod(_slop, _slop, _ecc_p); // slop mod p

    mpz_pow_ui(_temp1, _slop, 2);
    mpz_mul_ui(_temp2, one_x, 2);
    mpz_sub(_temp3, _temp1, _temp2);
    mpz_mod(_temp3, _temp3, _ecc_p); // two_x mode p

    mpz_sub(_temp1, _temp3, one_x);
    mpz_mul(_temp2, _slop, _temp1);
    mpz_add(two_y, _temp2, one_y);
    mpz_mod(two_y, one_y, _ecc_p); // two_y mode p
    mpz_set(two_x, _temp3);

    mpz_clear(_temp1);
    mpz_clear(_temp2);
    mpz_clear(_temp3);
    mpz_clear(_slop);

    return 0;
}


int Crypt::_ecc_point_doubling(mpz_t one_x, mpz_t one_y) {
    //if (mpz_cmp_ui(one_y, 0) == 0 && mpz_cmp_ui(one_x, 0) == 0 ) return;
    if (mpz_cmp_ui(one_y, 0) == 0 ) { 
        //mpz_set_ui(one_x, 0);
        mpz_set_ui(one_x, 0);
        mpz_set_ui(one_y, 0);
        return 1;
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
    //mpz_fdiv_q(_slop, _temp1, _temp2);
    mpz_mod(_slop, _slop, _ecc_p); // slop mod p

    mpz_pow_ui(_temp1, _slop, 2);
    mpz_mul_ui(_temp2, one_x, 2);
    mpz_sub(_temp3, _temp1, _temp2);
    mpz_mod(_temp3, _temp3, _ecc_p); // two_x mode p

    mpz_sub(_temp1, one_x, _temp3);
    mpz_mul(_temp2, _slop, _temp1);
    mpz_sub(one_y, _temp2, one_y);
    mpz_mod(one_y, one_y, _ecc_p); // two_y mode p
    mpz_set(one_x, _temp3);

    mpz_clear(_temp1);
    mpz_clear(_temp2);
    mpz_clear(_temp3);
    mpz_clear(_slop);
    return 0;
}


// three = one * two
void Crypt::_ecc_point_multiplication(mpz_t _K, mpz_t _ecc_t1, mpz_t _ecc_t2, mpz_t Q_x, mpz_t Q_y) {
    mpz_t _temp_k; mpz_init(_temp_k);
    mpz_set(_temp_k, _K);
    int * _A = _ecc_NAF(_temp_k);
    gmp_printf("_temp_k: %Zd\n", _temp_k);
    int lens = _A[0];
    //mpz_t Q_x; mpz_init(Q_x);
    //mpz_t Q_y; mpz_init(Q_y);
    mpz_t _temp_x; mpz_init(_temp_x);
    mpz_t _temp_y; mpz_init(_temp_y);
    mpz_t _temp_ecc_t2; mpz_init(_temp_ecc_t2);
    mpz_mul_si(_temp_ecc_t2, _ecc_t2, -1);
    mpz_mod(_temp_ecc_t2, _temp_ecc_t2, _ecc_p);
    cout << "lens: " << lens << endl;
    gmp_printf("_temp_ecc_t2: %Zd\n", _temp_ecc_t2);
    mpz_t atemp; mpz_init(atemp);
    mpz_t _atemp; mpz_init(_atemp);
    int flag = 1, _flag = -1;
    for (int i = lens-1 ; i >= 1; i-- ){
        mpz_ui_pow_ui(_atemp, 2, i-1);
        //mpz_mul_ui(_atemp, _atemp, _A[i]);
        if (_A[i] ==  1) 
            mpz_add(atemp, atemp, _atemp);
        else if (_A[i] == -1) 
            mpz_sub(atemp, atemp, _atemp);
        cout << "i: " << i << " A: " <<_A[i] <<" " <<endl;;
        gmp_printf("Q_x 1: %Zd\n", Q_x);
        gmp_printf("Q_y 1: %Zd\n", Q_y);
        if (mpz_cmp_ui(Q_y, 0) == 0) {
            flag = 1;
        }
        else{
            _flag = _ecc_point_doubling(Q_x, Q_y); //, _temp_x, _temp_y);
            flag = 0;
        }
        gmp_printf("Q_x 2: %Zd\n", Q_x);
        gmp_printf("Q_y 2: %Zd\n", Q_y);
        if (_A[i] == 1)  {
            if ( i == lens - 1 || flag) {
                mpz_set(Q_x, _ecc_t1);
                mpz_set(Q_y, _ecc_t2);
                gmp_printf("Q_x 31: %Zd\n", Q_x);
                gmp_printf("Q_y 31: %Zd\n", Q_y);
                flag = 0;
            }
            else{
                _flag = _ecc_point_addition(Q_x, Q_y, _ecc_t1, _ecc_t2, _temp_x, _temp_y);
                if (_flag){
                }
                else{
                    mpz_set(Q_x, _temp_x);
                    mpz_set(Q_y, _temp_y);
                    gmp_printf("Q_x 32: %Zd\n", Q_x);
                    gmp_printf("Q_y 32: %Zd\n", Q_y);
                }
            }
        }
        else if (_A[i] == -1) {
            if (i == lens - 1 || flag){
                mpz_set(Q_x, _ecc_t1);
                mpz_set(Q_y, _temp_ecc_t2);
                gmp_printf("Q_x 41: %Zd\n", Q_x);
                gmp_printf("Q_y 41: %Zd\n", Q_y);
                flag = 0;
            }
            else {
                _flag = _ecc_point_addition(Q_x, Q_y, _ecc_t1, _temp_ecc_t2, _temp_x, _temp_y);
                if (_flag){

                }
                else{
                    mpz_set(Q_x, _temp_x);
                    mpz_set(Q_y, _temp_y);
                    gmp_printf("Q_x 42: %Zd\n", Q_x);
                    gmp_printf("Q_y 42: %Zd\n", Q_y);
                }
            }
        }
    } cout << endl;
    gmp_printf("atemp: %Zd\n", atemp);
    gmp_printf("_K: %Zd\n", _K);
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
    gmp_printf(" _k : %Zd\n", _K);
    unsigned long int _quot;
    while(mpz_cmp_ui(_K, 1) >= 0) {
        if (mpz_odd_p(_K) != 0) {
            mpz_mod_ui(_temp, _K, 4);
            _naf[i] = 2 - (int)mpz_get_si(_temp) ;
            //cout <<"naf " << _naf[i] << endl;
            //gmp_printf(" _k 1: %Zd\n", _K);
            if (_naf[i] > 0)
                mpz_sub_ui(_K, _K, _naf[i]) ;
            else if (_naf[i] < 0) 
                mpz_add_ui(_K, _K, -1*_naf[i]);
            //mpz_sub_si(_K, _K, _naf[i]) ;

            //gmp_printf(" _k 2: %Zd\n", _K);
        }else {
            _naf[i] = 0;
        }
        _quot = mpz_fdiv_q_ui(_K, _K, 2) ;
        //gmp_printf(" _k 3: %Zd\n", _K);
        //cout <<"_quot: " << _quot << endl;
        i += 1;
    }
    _naf[0] = i;
    for (int k = 0 ; k < i; k ++) {
        cout << _naf[k] << " " ;
    }cout << endl;
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
    
    cout <<"while" << endl;
    int i = 0;
    while(mpz_cmp_ui(_ecc_embed_k, i) >= 0) {
        mpz_pow_ui(_temp_m2, _mess_x, 3);
        gmp_printf("_temp_m2 1: %Zd\n", _temp_m2);
        mpz_mul(_temp_m3, _mess_x, _ecc_a);
        gmp_printf("_temp_m3 2: %Zd\n", _temp_m3);
        mpz_add(_temp_m2, _temp_m2, _temp_m3);
        gmp_printf("_temp_m2 3: %Zd\n", _temp_m2);
        mpz_add(_temp_m2, _temp_m2, _ecc_b);
        gmp_printf("_temp_m2 4: %Zd\n", _temp_m2);
        mpz_powm(_temp_m3, _temp_m2, _remd1, _ecc_p);
        gmp_printf("_temp_m3 5: %Zd\n", _temp_m3);
        if (mpz_cmp_ui(_temp_m3, 0) > 0) {
            mpz_powm(_mess_y, _temp_m2, _remd2, _ecc_p);
            gmp_printf("_mess_y 6: %Zd\n", _mess_y);
            gmp_printf("_mess_x 7: %Zd\n", _mess_x);
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

    gmp_printf("_ecc_d_key 1: %Zd\n", _ecc_d_key);
    gmp_printf("_ecc_Gx 1: %Zd\n", _ecc_Gx);
    gmp_printf("_ecc_Gy 1: %Zd\n", _ecc_Gy);
    gmp_printf("_ecc_Qx_key 1: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 1: %Zd\n", _ecc_Qy_key);
    //mpz_set_str(_ecc_d_key, "1", 10);
    _ecc_point_multiplication(_ecc_d_key, _ecc_Gx, _ecc_Gy, _ecc_Qx_key, _ecc_Qy_key);
    //mpz_set(_ecc_public_keys._X, _ecc_Qx_key);
    //mpz_set(_ecc_public_keys._Y, _ecc_Qy_key);
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
    cout << "plain " << endl;
    _ecc_plain_text_embeded(message, _mess_x, _mess_y);
    gmp_printf("_mess_x : %Zd\n", _mess_x);
    gmp_printf("_mess_y : %Zd\n", _mess_y);
    //point ecc_point; 
    //mpz_set(ecc_point._X, _mess_x);
    //mpz_set(ecc_point._Y, _mess_y);
    //ecc_point.is_infinity = 0;
    cout << "plain 2" << endl;

    mpz_t _temp_n; mpz_init(_temp_n);
    mpz_t _temp_k; mpz_init(_temp_k);
    mpz_sub_ui(_temp_n, _ecc_n, 1);
    mpz_urandomm(_temp_k, _gstate, _temp_n);
    mpz_add_ui(_temp_k, _temp_k, 1);

    mpz_t _temp_c2_x; mpz_init(_temp_c2_x); 
    mpz_t _temp_c2_y; mpz_init(_temp_c2_y); 
    
    gmp_printf("_ecc_Qx_key 1: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 1: %Zd\n", _ecc_Qy_key);
    _ecc_point_multiplication(_temp_k, _ecc_Qx_key, _ecc_Qy_key, C1_x, C1_y); // c1 = kp
    gmp_printf("_ecc_Qx_key 2: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 2: %Zd\n", _ecc_Qy_key);

    _ecc_point_multiplication(_temp_k, _ecc_Qx_key, _ecc_Qy_key, _temp_c2_x, _temp_c2_y); // c2 = M + kp
    gmp_printf("_ecc_Qx_key 3: %Zd\n", _ecc_Qx_key);
    gmp_printf("_ecc_Qy_key 3: %Zd\n", _ecc_Qy_key);

    _ecc_point_addition(_mess_x, _mess_y, _temp_c2_x, _temp_c2_y, C2_x, C2_y); 
    gmp_printf("_mess_x 2: %Zd\n", _mess_x);
    gmp_printf("_mess_y 2: %Zd\n", _mess_y);

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
    mpz_mul_si(_temp_y, _temp_y, -1);
    mpz_mod(_temp_y2, _temp_y, _ecc_p);
    _ecc_point_addition(C2_x, C2_y, _temp_x, _temp_y2, mess_x, mess_y);
    mpz_fdiv_q(_message, mess_x, _ecc_embed_k);
    gmp_printf("_message: %Zd\n", _message);
    
    mpz_clear(_temp_x);
    mpz_clear(_temp_y);
    mpz_clear(mess_x);
    mpz_clear(mess_y);
    mpz_clear(_message);
}

void Crypt::ECC_Sign() {

}

int Crypt::ECC_Verify_Sign() {
    
    return 0;
}
