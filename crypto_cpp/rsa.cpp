#include "bigint.h"

void RSA::RSA_Plain_Generate_Keys(int p_length, int q_length) {
    string p = generate_prime(p_length);
    string q = generate_prime(q_length);
    string n = multiply(p, q);
    string phi = multiply(pos_sub(p, "1"), pos_sub(q, "1"));
    string e = _stored_e_keys[0];
    string d = modulo_inverse(e, phi) ;
    _public_keys[0] = e; _public_keys[1] = n;
    _private_keys[0] = d; _private_keys[1] = n;

    cout << "public keys,  e: " << e << " n: " << n << endl;
    cout << "private keys, d: " << d << " n: " << n << endl;
}

string RSA::RSA_Plain_Encryption(string message) {
    string C = _modular_pow(message, _public_keys[0], _public_keys[1]) ;
    return C;
}

string RSA::RSA_Plain_Decryption(string C) {
    string M = _modular_pow(C, _private_keys[0], _private_keys[1]);
    return M ;
}

string RSA::RSA_Plain_Sign(string message) {
    MD5_init();
    string hash_tag = MD5_value(message) ;
    cout << "hash tag -- : " << hash_tag << endl;
    string signature = _modular_pow( _hex2dec(hash_tag), _private_keys[0], _private_keys[1]) ;
    return signature;
} 

int RSA::RSA_Plain_Verify_Sign(string message, string signature) {
    MD5_init();
    string hash_tag = MD5_value(message) ;
    string hash_tag_dec = _hex2dec(hash_tag);
    string hash_tag_ = _modular_pow( signature, _public_keys[0], _public_keys[1]) ; 

    cout << "hash_tag: " << hash_tag << endl;
    cout << "hash_tag_dec: " << hash_tag_dec << endl;
    cout << "hash_tag_: " << hash_tag_ << endl;

    if (hash_tag_dec.compare(hash_tag_) == 0) {
        cout << " accept the signature " << endl;
        return 1;
    }else {
        cout << " reject the signature " << endl;
        return 0;
    }
}

