#include "hash_algorithms.h"


void HashAlgorithms::_get_MD5_K() {
    for (int i = 0; i < HASH_LEN; i++ ) {
        MD5_K[i] = -1;
    }
}
string HashAlgorithms::_dec2hex(unsigned int _temp, int _len) {
    
    stringstream ioss; string s_temp;
    ioss << hex << _temp;
    ioss >> s_temp;
    if (s_temp.length() < _len) {
        string _zero("0", _len - s_temp.length());
        s_temp = _zero + s_temp;
    }
    return s_temp;
}

string HashAlgorithms::_hex2dec(string _temp) {

    string _res = "";
    for (int i = 0; i < _temp.length(); i++) {
        _res += to_string((int)_temp.at(i));
    }
    return _res;
}


void HashAlgorithms::MD5_init() {

    int _MD5_S[HASH_LEN] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21}; 

    for (int i = 0; i < HASH_LEN; i++) {
        MD5_S[i] = _MD5_S[i];
    }
    MD5_K[0] = 0xd76aa478; MD5_K[1] = 0xe8c7b756; MD5_K[2] = 0x242070db; MD5_K[3] = 0xc1bdceee;
    MD5_K[4] = 0xf57c0faf; MD5_K[5] = 0x4787c62a; MD5_K[6] = 0xa8304613; MD5_K[7] = 0xfd469501;
    MD5_K[8] = 0x698098d8; MD5_K[9] = 0x8b44f7af; MD5_K[10] = 0xffff5bb1; MD5_K[11] = 0x895cd7be;
    MD5_K[12] = 0x6b901122; MD5_K[13] = 0xfd987193; MD5_K[14] = 0xa679438e; MD5_K[15] = 0x49b40821;
    MD5_K[16] = 0xf61e2562; MD5_K[17] = 0xc040b340; MD5_K[18] = 0x265e5a51; MD5_K[19] = 0xe9b6c7aa;
    MD5_K[20] = 0xd62f105d; MD5_K[21] = 0x02441453; MD5_K[22] = 0xd8a1e681; MD5_K[23] = 0xe7d3fbc8;
    MD5_K[24] = 0x21e1cde6; MD5_K[25] = 0xc33707d6; MD5_K[26] = 0xf4d50d87; MD5_K[27] = 0x455a14ed;
    MD5_K[28] = 0xa9e3e905; MD5_K[29] = 0xfcefa3f8; MD5_K[30] = 0x676f02d9; MD5_K[31] = 0x8d2a4c8a;
    MD5_K[32] = 0xfffa3942; MD5_K[33] = 0x8771f681; MD5_K[34] = 0x6d9d6122; MD5_K[35] = 0xfde5380c;
    MD5_K[36] = 0xa4beea44; MD5_K[37] = 0x4bdecfa9; MD5_K[38] = 0xf6bb4b60; MD5_K[39] = 0xbebfbc70;
    MD5_K[40] = 0x289b7ec6; MD5_K[41] = 0xeaa127fa; MD5_K[42] = 0xd4ef3085; MD5_K[43] = 0x04881d05;
    MD5_K[44] = 0xd9d4d039; MD5_K[45] = 0xe6db99e5; MD5_K[46] = 0x1fa27cf8; MD5_K[47] = 0xc4ac5665;
    MD5_K[48] = 0xf4292244; MD5_K[49] = 0x432aff97; MD5_K[50] = 0xab9423a7; MD5_K[51] = 0xfc93a039;
    MD5_K[52] = 0x655b59c3; MD5_K[53] = 0x8f0ccc92; MD5_K[54] = 0xffeff47d; MD5_K[55] = 0x85845dd1;
    MD5_K[56] = 0x6fa87e4f; MD5_K[57] = 0xfe2ce6e0; MD5_K[58] = 0xa3014314; MD5_K[59] = 0x4e0811a1;
    MD5_K[60] = 0xf7537e82; MD5_K[61] = 0xbd3af235; MD5_K[62] = 0x2ad7d2bb; MD5_K[63] = 0xeb86d391;

    MD5_a0 = 0x67452301; MD5_b0 = 0xefcdab89; MD5_c0 = 0x98badcfe; MD5_d0 = 0x10325476; 
    //cout << "MD5_a0: " << MD5_a0 << endl; 
    //cout << "MD5_b0: " << MD5_b0 << endl; 
    //cout << "MD5_c0: " << MD5_c0 << endl; 
    //cout << "MD5_d0: " << MD5_d0 << endl; 
}

int * HashAlgorithms::_add_char1( string message) {
    int num = ((message.length() + 8) / 64) + 1, i;
    int * strByte = new int[num * 16];
    for (i = 0; i < num * 16; i++)
        strByte[i] = 0;
    for (i = 0; i < message.length(); i++) {
        strByte[i >> 2] |= message.at(i) << ((i % 4) * 8);
    }
    strByte[i >> 2] |= 0x80 << ((i % 4) * 8);
    strByte[num * 16 - 1] = message.length() * 8;
    return strByte;
}

int * HashAlgorithms::_add_char2( string message) {
    int num = ((message.length() + 8) / 64) + 1, i;
    int * strByte = new int[num * 16];
    for (i = 0; i < num * 16; i++)
        strByte[i] = 0;
    for (i = 0; i < message.length(); i++) {
        strByte[i >> 2] |= message.at(i) << ((i % 4) * 8);
    }
    strByte[i >> 2] |= 0x80 << ((i % 4) * 8);
    strByte[num * 16 - 2] = message.length() * 8;
    return strByte;

}

int HashAlgorithms::_left_shift(int a, int s) {
    if (s > 32) s %= 32;
    unsigned int a_ = a;
    return (int) (a_ << s ) | (a_ >> (32 - s) );
}

void HashAlgorithms::_little_endian(int *data, int len) {
    for (int i = 0; i < len; i++) {
        *data = ((*data & 0xff000000) >> 24) 
              | ((*data & 0x00ff0000) >> 8)
              | ((*data & 0x0000ff00) << 8)
              | ((*data & 0x000000ff) << 24);
        data ++ ;
    }
}

void HashAlgorithms::_big_endian(int *data, int len) {
    for (int i = 0; i < len; i++) {
        *data = __builtin_bswap32(*data);
        data ++;
    }
}

string HashAlgorithms::MD5_value(string message) {
    cout << "me len : " << message.length() << endl;
    cout << "message : " << message << endl;
    int len, fill_bit, fill_byte, new_len;
    len = message.length() ; 
    fill_bit = 448 - (len * 8 % 512 ) % 448 ;
    fill_byte = fill_bit / 8;
    new_len = (len + fill_byte + 8) / 4;

    int * new_message = _add_char2(  message);
    int chunk_len = new_len * 32 / 512;
    for (int i = 0; i < chunk_len; i++) {
        int MD5_A = MD5_a0; int MD5_B = MD5_b0;
        int MD5_C = MD5_c0; int MD5_D = MD5_d0;
        for (int j = 0; j < 64; j ++) {
            int MD5_F, MD5_g;
            if ( j < 16) {
                MD5_F = (MD5_B & MD5_C) | (~MD5_B & MD5_D) ;
                MD5_g = j;
            }else if (j < 32) {
                MD5_F = (MD5_D & MD5_B) | (~MD5_D & MD5_C) ;
                MD5_g = (5 * j + 1) % 16;
            }else if ( j < 48) {
                MD5_F = MD5_B ^ MD5_C ^ MD5_D;
                MD5_g = (3 * j + 5) % 16;
            }else {
                MD5_F = MD5_C ^ (MD5_B | ~MD5_D);
                MD5_g = (7 * j ) % 16;
            }
            MD5_F = MD5_F + MD5_A + MD5_K[j] + new_message[ i * chunk_len + MD5_g];
            MD5_A = MD5_D; MD5_D = MD5_C;
            MD5_C = MD5_B; MD5_B = MD5_B + _left_shift(MD5_F, MD5_S[j]);
        }
        MD5_a0 = MD5_a0 + MD5_A;
        MD5_b0 = MD5_b0 + MD5_B;
        MD5_c0 = MD5_c0 + MD5_C;
        MD5_d0 = MD5_d0 + MD5_D;
    }
    delete [] new_message;
    _little_endian( &MD5_a0, 1); _little_endian(&MD5_b0, 1); _little_endian(&MD5_c0, 1); _little_endian(&MD5_d0, 1);
    return _dec2hex((unsigned int)MD5_a0, 8) + _dec2hex((unsigned int)MD5_b0, 8) + _dec2hex((unsigned int)MD5_c0, 8) + _dec2hex((unsigned int)MD5_d0, 8);

}

void HashAlgorithms::sha1_init() {
    sha1_h0 = 0x67452301;
    sha1_h1 = 0xEFCDAB89;
    sha1_h2 = 0x98BADCFE;
    sha1_h3 = 0x10325476;
    sha1_h4 = 0xC3D2E1F0;

}
string HashAlgorithms::sha1_value(string message) {
    cout << "message length: " << message.length() << endl;
    cout << "message: " << message << endl;   
    int len, fill_bit, fill_byte, new_len, temp_len = 80;
    len = message.length();
    fill_bit = 448 - (len * 8 % 512) % 448;
    fill_byte = fill_bit / 8;
    new_len = (len + fill_byte + 8) / 4;

    int * new_message = _add_char1(message); 
    for (int i = 0; i < new_len; i++) {
        cout << new_message[i] << " ";
        if ( i != 0 && i % 16 == 0) cout << endl;
    }
    cout << endl;
    _big_endian(new_message, new_len);
    int chunk_len = new_len * 32 / 512;
    int * temp_message = new int[temp_len];
    int a, b, c, d, e, f, k, temp;
    
    for (int i = 0; i < chunk_len; i++) {
        for (int j = 0; j < temp_len; j ++) {
            temp_message[j] = 0;
        }
        for (int j = 0; j < temp_len; j ++) {
            if ( j < 16) temp_message[j] = new_message[i * chunk_len + j];
            else 
                temp_message[j] = _left_shift(temp_message[j - 3] ^ temp_message[j - 8] ^ temp_message[j - 14] ^ temp_message[j - 16], 1);
        }
        a = sha1_h0; b = sha1_h1; c = sha1_h2; d = sha1_h3; e = sha1_h4;
        for (int j = 0; j < temp_len; j ++) {
            if (j < 20) {
                f = (b & c) | ((!b) & d) ;
                k = 0x5A827999;
            }else if ( j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            temp = _left_shift(a, 5) + f + e + k + temp_message[j];
            e = d; d = c; c = _left_shift(b, 30); b = a; a = temp;
        }
        sha1_h0 += a;
        sha1_h1 += b;
        sha1_h2 += c;
        sha1_h3 += d;
        sha1_h4 += e;
    }
    delete temp_message;
    //int hh = (sha1_h0 << 128) | (sha1_h1 << 96) | (sha1_h2 << 64) | (sha1_h3 << 32) | sha1_h4;
    string hh;
    _big_endian(&sha1_h0, 1);
    _big_endian(&sha1_h1, 1);
    _big_endian(&sha1_h2, 1);
    _big_endian(&sha1_h3, 1);
    _big_endian(&sha1_h4, 1);
    hh = _dec2hex((unsigned int)sha1_h0, 8) + _dec2hex((unsigned int)sha1_h1, 8) + _dec2hex((unsigned int)sha1_h2, 8) + _dec2hex((unsigned int) sha1_h3, 8) + _dec2hex((unsigned int) sha1_h4, 8);
    //return to_string(hh);
    return hh;
}
