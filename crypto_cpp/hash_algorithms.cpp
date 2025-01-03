#include "bigint.h"

//HashAlgorithms::HashAlgorithms(void) {}

//HashAlgorithms::~HashAlgorithms(void) {}

void HashAlgorithms::_get_MD5_K() {
    for (int i = 0; i < HASH_LEN; i++ ) {
        MD5_K[i] = -1;
    }
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
    cout << "MD5_a0: " << MD5_a0 << endl; 
    cout << "MD5_b0: " << MD5_b0 << endl; 
    cout << "MD5_c0: " << MD5_c0 << endl; 
    cout << "MD5_d0: " << MD5_d0 << endl; 
}


//int * HashAlgorithms::_add_char(char* message) {
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
void HashAlgorithms::_add_char( int *new_message, string message, int new_len, int len, int fill_byte) {
    bitset<8> w1, w2, w3, w4, w5, w6, w7, w8;
    char * _temp = new char[new_len * 4];
    //for (int i = 0; i < len; i++) {
    //    new_message[i] = message.at(i)
    //};
    strcpy(_temp, message.c_str());
    _temp[len] = 0x80;
    //new_message[len] = 0x80;
    //for (int i = 0; i < fill_byte - 1; i++) {
    //    new_message[len + i + 1] = 0x00;

    //}
    memset(_temp + len + 1, 0x00, fill_byte - 1); 
    bitset<64> wo(len * 8) ;
    int ww[8] ;
    for (int i = 0; i < 64; i++) {
        if( i < 8)
            w1[i] = wo[i];
        else if( i>= 8 && i < 16) {
            ww[0] = (int) w1.to_ulong();
            w2[i - 8] = wo[i];
        }
        else if(i>= 16 && i < 24) {
            ww[1] = (int) w2.to_ulong();
            w3[i - 16] = wo[i]; 
        }
        else if( i >= 24 && i < 32) {
            ww[2] = (int) w3.to_ulong();
            w4[i - 24] = wo[i]; 
        }
        else if (i >= 32 && i < 40) {
            ww[3] = (int) w4.to_ulong();
            w5[i - 32] = wo[i]; 
        }
        else if( i >= 40 && i < 48) {
            ww[4] = (int) w5.to_ulong();
            w6[i - 40] = wo[i]; 
        }
        else if( i >= 48 && i < 56) {
            ww[5] = (int) w6.to_ulong();
            w7[i - 48] = wo[i]; 
        }
        else if( i >= 56 && i < 64) {
            ww[6] = (int) w7.to_ulong();
            w8[i - 56] = wo[i]; 
        }
    }
    ww[7] = (int) w8.to_ulong();
    for (int i = 0; i < 8; i++) {
        //new_message[len + fill_byte + i] = ww[8 - i - 1];
        _temp[len + fill_byte + i] = ww[8 - i - 1] ; 
    }
    cout << "_temp: " << _temp << endl;
    cout << "new message: " << new_message << endl;
    for (int i = 0; i < new_len; i++) {
        //cout << bitset<8>(new_message[i]) << endl;
        new_message[i] = _temp[i*4+3] + (_temp[i*4+2]<<8) + (_temp[i*4+1]<<16) + (_temp[i * 4 ]<<24) ;
    }
    cout << "new len is: " << sizeof(new_message) / sizeof(new_message[0]) << endl;
    cout << " size of : " << sizeof(new_message) << " " << sizeof(new_message[0]) << endl;
    delete [] _temp;
    //return new_message;
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

string HashAlgorithms::MD5_value(string message) {
    cout << "me len : " << message.length() << endl;
    cout << "message : " << message << endl;
    //char * new_message = _add_char(temp2);
    int len, fill_bit, fill_byte, new_len;
    len = message.length() ; //sizeof(message) / sizeof(message[0]);
    fill_bit = 448 - (len * 8 % 512 ) % 448 ;
    fill_byte = fill_bit / 8;
    new_len = (len + fill_byte + 8) / 4;

    //int * new_message = new int[new_len]; //_add_char(temp2);
    //_add_char( new_message, message, new_len, len, fill_byte);
    int * new_message = _add_char2(  message);

    //cout << "new_message" << endl;
    //int new_len = sizeof(new_message) / sizeof(new_message[0]);
    //cout << "new_len: " << new_len << " strlen: " << strlen(new_message) << endl;
    //cout << "new len: " << new_len << endl;
    //cout << "new message: " << new_message << endl;
    //for (int i = 0; i < new_len; i++) {
    //    cout << bitset<32>(new_message[i]) << " " << new_message[i]<< endl;
    //}
    //_little_endian(new_message, new_len) ;
    //cout << "new message: " << new_message << endl;
    //for (int i = 0; i < new_len; i++) {
    //    cout << bitset<32>(new_message[i]) << " " << new_message[i]<< endl;
    //}
    int chunk_len = new_len * 32 / 512;
    //cout << "chunk len: " << chunk_len << endl;
    for (int i = 0; i < chunk_len; i++) {
        int MD5_A = MD5_a0; int MD5_B = MD5_b0;
        int MD5_C = MD5_c0; int MD5_D = MD5_d0;
        //cout << "md5 a b c d " << MD5_A << " " << MD5_B << " " << MD5_C << " " << MD5_D << endl;
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
            /**
            cout << "new mes: " << new_message[ i * chunk_len + MD5_g] << endl;
            cout << " md5 f: " << MD5_F << endl;
            cout << "left shift: " << _left_shift(MD5_F, MD5_S[j]) << endl;
            cout << " md5 a: " << MD5_A << endl;
            cout << " md5 b: " << MD5_B << endl;
            cout << " md5 c: " << MD5_C << endl;
            cout << " md5 d: " << MD5_D << endl;
            cout << " md5 g: " << MD5_g << endl;
            cout << " new_message[ i * chunk_len + MD5_g]: " << new_message[ i * chunk_len + MD5_g] << endl;
            cout << "_left_shift(MD5_F, MD5_S[j]): " << _left_shift(MD5_F, MD5_S[j]) << endl; */
        }
        //cout << "md5 a b c d " << MD5_A << " " << MD5_B << " " << MD5_C << " " << MD5_D << endl;
        MD5_a0 = MD5_a0 + MD5_A;
        MD5_b0 = MD5_b0 + MD5_B;
        MD5_c0 = MD5_c0 + MD5_C;
        MD5_d0 = MD5_d0 + MD5_D;
        //cout << "end one round .. " << endl;
    }
    delete [] new_message;
    //cout << "md5 a0 b0 c0 d0 " << MD5_a0 << " " << MD5_b0 << " " << MD5_c0 << " " << MD5_d0 << endl;
    //cout << "end md5" << endl;
    _little_endian( &MD5_a0, 1); _little_endian(&MD5_b0, 1); _little_endian(&MD5_c0, 1); _little_endian(&MD5_d0, 1);
    //cout << "md5 a0 b0 c0 d0 " << MD5_a0 << " " << MD5_b0 << " " << MD5_c0 << " " << MD5_d0 << endl;
    //return to_string(MD5_a0) + to_string(MD5_b0) + to_string(MD5_c0) + to_string(MD5_d0);
    return _dec2hex((unsigned int)MD5_a0, 8) + _dec2hex((unsigned int)MD5_b0, 8) + _dec2hex((unsigned int)MD5_c0, 8) + _dec2hex((unsigned int)MD5_d0, 8);

}


