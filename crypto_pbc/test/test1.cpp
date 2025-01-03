#include <pbc/pbc.h>
#include <gmpxx.h>
#include <iostream>
#include <cryptopp/sha.h>
#include <cryptopp/sm3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using namespace std;
using namespace CryptoPP;


int main()  {
    SHA256 sha256;
    SM3 sm3;
    pairing_t pairing;
    char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    cout << "count : " << count << endl;
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    
    element_t g, h;
    element_t public_key, secret_key;
    element_t sig;
    element_t temp1, temp2;
    element_init_G2(g, pairing);
    element_init_G2(public_key, pairing);
    element_init_G1(h, pairing);
    element_init_G1(sig, pairing);
    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);
    element_init_Zr(secret_key, pairing);

    element_random(g);
    element_random(secret_key);
    element_pow_zn(public_key, g, secret_key);
    
    string message = "hello";
    string hash_value;
    StringSource s2(message, true, new HashFilter(sm3, new HexEncoder(new StringSink(hash_value))));
    cout << "hash sm3: " << hash_value << endl;
    StringSource s(message, true, new HashFilter(sha256, new HexEncoder(new StringSink(hash_value))));
    char * _hash_value = new char[1000] ; // <const_cast> <char*>(hash_value.c_str());   
    strcpy(_hash_value, hash_value.c_str());
    cout << message << endl;
    cout << hash_value << endl;   
    element_from_hash(h, _hash_value, 32);
    element_pow_zn(sig, h, secret_key);

    pairing_apply(temp1, sig, g, pairing);
    pairing_apply(temp2, h, public_key, pairing);
    if (! element_cmp(temp1, temp2)) {
        cout << "signature verifies" << endl;
    }else
        cout <<"signature failed" << endl;
    delete _hash_value;
    element_printf("sig %B\n", sig);
    element_printf("temp1 %B\n", temp1);
    element_printf("h %B\n", h);
    element_printf("g %B\n", g);
    element_printf("secret key %B\n", secret_key);
}
