#include <iostream>
#include <string>
#include <stdio.h>
#include <bitset>
#include <cstring>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <inttypes.h>

using namespace std;

#define MAX_LEN 100000
#define PRIME_AMOUNT 9592
#define BASE 10
#define HASH_LEN 64


class HashAlgorithms {
    public:

        void MD5_init();
        string _dec2hex(unsigned int _temp, int _len);
        string _hex2dec(string _temp);

        string MD5_value(string message);

        void sha1_init();
        string sha1_value(string message);

    private:
        int MD5_S[HASH_LEN], MD5_K[HASH_LEN];
        int MD5_a0, MD5_b0, MD5_c0, MD5_d0;

        int * _add_char1( string message);
        int * _add_char2( string message);
        int _left_shift(int a, int s);
        void _get_MD5_K() ;

        void _little_endian(int *data, int len) ;
        void _big_endian(int *data, int len) ;

        int sha1_h0, sha1_h1, sha1_h2, sha1_h3, sha1_h4;
};


