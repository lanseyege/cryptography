#include "sm4.h"

SM4::SM4() {
    _fk = new unsigned int[4];
    _fk[0] = 0xa3b1bac6;
    _fk[1] = 0x56aa3350;
    _fk[2] = 0x677d9197;
    _fk[3] = 0xb27022dc;

    _rk = new unsigned int[len];
    _ck = new unsigned int[len] ;
    for (int i = 0; i < len; i++) {
        _ck[i] = ((4 * i + 0) * 7 % 256 << 24) + ((4 * i + 1) * 7 % 256 << 16) + ((4 * i + 2) * 7 % 256 << 8) + (4 * i + 3) * 7 % 256  ;
    }
}

SM4::~SM4() {
    delete _fk;
    delete _ck;
    delete _rk;
}
string SM4::_string_to_hex(string message) {
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


string SM4::_tohex_tostring(unsigned int inx) {
    ostringstream ss;
    ss << hex << inx;
    string res = ss.str();
    string _res(8 - res.length(), '0');
    return _res + res;
}
unsigned int SM4::_tau(unsigned int inx) {
    return (Sbox[(inx>>24) & 0xff] << 24) + (Sbox[(inx>>16) & 0xff] << 16) + (Sbox[(inx>>8) & 0xff] << 8) + Sbox[inx & 0xff] ;
}
unsigned int SM4::_left1(unsigned int inx) {
    return inx ^ ((inx << 2) | (inx>>30)) ^ ((inx << 10) | (inx >>22)) ^ ((inx << 18) | (inx >> 14)) ^ ((inx << 24) | (inx >>8)) ;
}
unsigned int SM4::_left2(unsigned int inx) {
    return inx ^ ( (inx << 13) | (inx >> 19)) ^ ((inx << 23) | (inx >> 9)) ;
}

void SM4::pk_exte(string private_key) {
    unsigned int mk0 = strtol(private_key.substr(0, 8).c_str(), 0, 16);
    unsigned int mk1 = strtol(private_key.substr(8, 8).c_str(), 0, 16);
    unsigned int mk2 = strtol(private_key.substr(16, 8).c_str(), 0, 16);
    unsigned int mk3 = strtol(private_key.substr(24, 8).c_str(), 0, 16);
    //cout <<"mk: "<<mk0 << " " << mk1 << " " << mk2 << " " << mk3 << endl;
    //printf("mk: %x %x %x %x\n", mk0, mk1, mk2, mk3);
    unsigned int mid;
    unsigned int * _k = new unsigned int[4];
    _k[0] = mk0 ^ _fk[0];
    _k[1] = mk1 ^ _fk[1];
    _k[2] = mk2 ^ _fk[2];
    _k[3] = mk3 ^ _fk[3];
    for ( int i = 0; i < len; i++ ) {
        mid = _k[(i+1) % 4] ^ _k[(i+2) % 4] ^ _k[(i+3) % 4] ^ _ck[i] ;
        _rk[i] = _k[i % 4] ^ _left2(_tau(mid)) ;
        _k[(i+4) % 4] = _rk[i] ;
        //cout <<"_rk " << i << " " << _rk[i] << endl; 
        //printf("_rk[%d]: %x\n", i, _rk[i]);
    }
    delete _k;
}

string SM4::en_de_crypt(string message, int flag) {
    int mlen = message.length();
    int kn = mlen / 32 , kr = mlen % 32; 
    //if (kr != 0) kn ++;
    string all_crypts ;
    unsigned int * _x = new unsigned int[4];
    for (int inx = 0; inx < kn; inx ++) {
        _x[0] = strtol(message.substr(inx * 32 , 8).c_str(), 0, 16);
        _x[1] = strtol(message.substr(inx * 32 + 8, 8).c_str(), 0, 16);
        _x[2] = strtol(message.substr(inx * 32 + 16, 8).c_str(), 0, 16);
        _x[3] = strtol(message.substr(inx * 32 + 24, 8).c_str(), 0, 16);
        all_crypts += _en_de_crypt(_x, flag);
    }
    if (kr != 0) {
        _x[0] = 0x00; _x[1] = 0x00; _x[2] = 0x00; _x[3] = 0x00;
        int _kr = kr / 8;
        if (kr % 8 != 0) _kr += 1;
        for (int j = 0; j < _kr; j++)
            _x[j] = strtol(message.substr(kn * 32 + 8 * j, 8).c_str(), 0, 16);
        all_crypts += (_en_de_crypt(_x, flag));
    }
    return all_crypts;
}

string SM4::_en_de_crypt(unsigned int * _x, int flag) {
    unsigned int mid;
    int  _i = 0;
    for (int i = 0; i < len; i++) {
        if (flag) _i = len - i - 1; 
        else _i = i;
        mid = _x[ (i+1)%4] ^_x[ (i+2)%4] ^ _x[ (i+3)%4] ^ _rk[_i];
        _x[(i+4)%4] = _x[i % 4] ^ _left1(_tau(mid));
        //cout << " x " << i <<" : " <<  _x[(i+4)%4] << endl;
        //printf("_x[%d]: %x\n", i+4, _x[(i+4)%4]);
    }
    string crypts = "";
    crypts = _tohex_tostring(_x[3]) + _tohex_tostring(_x[2]) + _tohex_tostring(_x[1]) + _tohex_tostring(_x[0]);
    
    return crypts;
}

void SM4::sm4_test() {
    printf("_fk %x %x\n", _fk[0], _fk[1]);
    printf("_ck %x %x\n", _ck[0], _ck[31]);
    int a = 0x08; 
    printf("a: %x\n", a << 8);
    printf("a: %d\n", a << 8);
}
/**
int main() {
    SM4 *sm4 = new SM4();
    sm4->sm4_test(); 
    string pk = "0123456789abcdeffedcba9876543210"  ;
    string message = "0123456789abcdeffedcba9876543210" ; 
    message = sm4->_string_to_hex(message);
    cout << "message len: " << message.length() << endl;
    sm4->pk_exte(pk);

    string cyber = sm4->en_de_crypt(message, 0);
    cout << cyber << endl;
    string text = sm4->en_de_crypt(cyber, 1);
    cout << text << endl;
} **/
