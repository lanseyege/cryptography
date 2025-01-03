#include <iostream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <cstring>

using namespace std;

typedef unsigned char BYTE;

int md5(const char* mstr) {
    int mlen = strlen(mstr);
    int fill_size = 448 - ((mlen * 8) % 512) % 448;
    int fill_byte = fill_size / 8;
    int new_mlen = mlen + fill_byte + 8;

    char *newstr = new char[new_mlen] ;
    strcpy(newstr, mstr);
    memset(newstr + mlen, 0x80, 1);
    memset(newstr + mlen + 1 , 0x00, fill_byte - 1);
    bitset<64> wo(mlen * 8);
    char* ch = const_cast<char*>(wo.to_string().c_str());
    cout << "ch: " << ch << " " << wo.to_string() << endl;
    strcpy(newstr + mlen + fill_byte, ch);
    cout <<"mlen: " << mlen<< endl;    
    //memset(newstr + mlen + fill_byte, (long long) mlen, 8);a
    //CopyMemory(newstr, mstr, mlen);

    //newstr[mlen] = 0x80;
    //ZeroMemory(&newstr[mlen + 1], fill_byte - 1);
    long long lenbit = mlen * 8ll;
    //CopyMemory(&newstr[mlen + fill_byte], &lenbit, 8);
    cout << "new str: " << endl;
    cout << newstr ;
    cout <<"hehe"<<endl;
    for(int i = 0; i < new_mlen; i++) {
        cout << newstr[i] << endl;
    }

    delete[] newstr ;

    return 0;
}

int md5_1(const char* mstr) {
    int len, fill_bit, fill_byte, new_len;
    len = strlen(mstr);
    fill_bit = 448 - ((len * 8)%512) % 448; 
    fill_byte = fill_bit / 8;
    new_len = len + fill_bit / 8 + 8;
    cout << "len: " << len << " new len: " << new_len << endl; 
    long long aa = len * 8;   
    char *newstr = new char[new_len];
    //memset(newstr, mstr, len);
    strcpy(newstr, mstr);
    memset(newstr + len, 0x80, 1);
    memset(newstr + len + 1, 0x00, fill_byte - 1);
    //memset(newstr + len + fill_byte, (int)(bitset<64>(len * 8).to_ulong()), 8);
    
    std::bitset<64> wo(len * 8) ;
    cout << wo <<endl;
    cout << wo[0] << " " << wo[1] << " " << wo[2] << " " << wo[3] << " " << wo[4] << endl;
    bitset<8> w1, w2, w3, w4, w5, w6, w7, w8;
    for(int i = 0; i < 64; i ++)  {
            if( i < 8)
                w1[i] = wo[i];
            else if( i>= 8 && i < 16)
                w2[i - 8] = wo[i];
            else if(i>= 16 && i < 24)
                w3[i - 16] = wo[i];
            else if( i >= 24 && i < 32)
                w4[i - 24] = wo[i];
            else if (i >= 32 && i < 40)
                w5[i - 32] = wo[i];
            else if( i >= 40 && i < 48)
                w6[i - 40] = wo[i];
            else if( i >= 48 && i < 56)
                w7[i - 48] = wo[i];
            else if( i >= 56 && i < 64)
                w8[i - 56] = wo[i];
        
    }
    cout << w1 << " " << (int) w1.to_ulong() << endl;
    cout << w2 << endl;
    cout << w3 << endl;
    cout << w4 << endl;
    cout << w5 << endl;
    cout << w6 << endl;
    cout << w7 << endl;
    cout << w8 << endl;
    memset(newstr + len + fill_byte, (int)(w8.to_ulong()), 1);
    memset(newstr + len + fill_byte + 1, (int)(w7.to_ulong()), 1);
    memset(newstr + len + fill_byte + 2, (int)(w6.to_ulong()), 1);
    memset(newstr + len + fill_byte + 3, (int)(w5.to_ulong()), 1);
    memset(newstr + len + fill_byte + 4, (int)(w4.to_ulong()), 1);
    memset(newstr + len + fill_byte + 5, (int)(w3.to_ulong()), 1);
    memset(newstr + len + fill_byte + 6, (int)(w2.to_ulong()), 1);
    memset(newstr + len + fill_byte + 7, (int)(w1.to_ulong()), 1);
    cout <<"new str: " << newstr << endl;
    for(int i = 0; i < new_len; i++) {
        cout << bitset<8>(newstr[i]) <<endl;
    }
    cout <<endl;
    cout <<"end"<<endl;

    bitset<4> ww(25);
    cout << ww << endl;

    return 0;
}

int main() {
    string str = "I am your father, my son!!!";
    BYTE bytes = strtoul(str.c_str(), NULL, 16);

    cout << "bytes: "<< bytes <<endl;
    
    int byte = int(bytes);
    cout <<" byte: " << byte << endl;
    //string str = "tech";
    for (char const & c : str) {
        cout << bitset<8>(c) << " " ;
    }
    cout << endl;
    
    int a = 100;
    int b = 200;
    b = 448 - (a * 8 % 512) %448;
    cout << b << endl;
    //str = str + 0x80;
    cout << "new str: " << str << endl;
    char t1 = 0x80;
    cout << t1 << endl;
    t1 = 0x00;
    cout << t1 << endl;
    cout << sizeof(string) << endl;
    cout << sizeof(str) << endl;

    cout << sizeof(long) <<" " << sizeof(long long) << " " << sizeof(int) << " " << sizeof(unsigned long long) <<endl;
    //string ke = "hello";
    md5("hello") ;
    //md5_1("hello");
}



