#include <iostream>
//#include <fstream>
//#include <cassert>
//#include <string>

#include "bigint.h"

using namespace std;

BigInt::BigInt(void) {
    BigInt::division_value = "";
    BigInt::modulo_value = "";
    _BASE = to_string(BASE);
    _is_get_stored_primes_ = 0; 
}

BigInt::~BigInt(void) {}

string BigInt::multiply(string left, string right) {
    //cout <<"multiply left: "<<left<<" right: "<<right<<endl;
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();

    char left_sign, right_sign;
    left_sign = left.at(0);
    right_sign = right.at(0);
    if (left_sign == '-' && right_sign == '-') {
        return karatsuba( left.substr(1, left_len), right.substr(1, right_len));
    }else if (left_sign != '-' && right_sign == '-') {
        return '-' + karatsuba( left, right.substr(1, right_len));
    }else if (left_sign == '-' && right_sign != '-') {
        return '-' + karatsuba( left.substr(1, left_len), right);
    }else {
        return karatsuba(left, right);
    }
}

string BigInt::add(string left, string right) {
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();

    char left_sign, right_sign;
    left_sign = left.at(0);
    right_sign = right.at(0);
    if (left_sign == '-' && right_sign == '-') {
        return "-" + plus( left.substr(1, left_len), right.substr(1, right_len));
    }else if (left_sign != '-' && right_sign == '-') {
        return sub(left, right.substr(1, right_len));
    }else if (left_sign == '-' && right_sign != '-') {
        return sub(right, left.substr(1, left_len));
    }else {
        return plus(left, right);
    }
}

void BigInt::division(string left, string right) {
    cout <<"division left: "<<left<<" right: "<<right<<endl;
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();

    char left_sign, right_sign;
    left_sign = left.at(0);
    right_sign = right.at(0);
    if (left_sign == '-' && right_sign == '-' ) {
        div(left.substr(1, left_len), right.substr(1, right_len));
    }else if (left_sign != '-' && right_sign != '-' ) {
        div(left, right);
    }else if (left_sign == '-' && right_sign != '-' ) {
        div(left.substr(1, left_len), right);
        division_value = "-" + division_value;
        if (compare(modulo_value, "0") != 0) modulo_value = pos_sub(right, modulo_value);
    }else if (left_sign != '-' && right_sign == '-' ) {
        div(left, right.substr(1, right_len));
        division_value = "-" + division_value;
        if (compare(modulo_value, "0") != 0) modulo_value = pos_sub(right.substr(1, right_len), modulo_value);
    }
    
    //return division_value;
    return ;
}

void BigInt::modulo(string left, string right) {
    div(left, right);
    //return modulo_value;
    return;
}

string BigInt::get_division_value() {
    return BigInt::division_value;
}

string BigInt::get_modulo_value() {
    return BigInt::modulo_value;
}

//  left / right = res ; left % right = modulo


void BigInt::div(string left, string right) {
    //cout <<" div left: "<<left<<" right: "<<right<<endl;
    right = get_substr(right, 0, right.length());
    if (right == "0") { BigInt::division_value = ""; BigInt::modulo_value = ""; return ;}
    if (compare(left, right) == 0) { BigInt::division_value = "1"; BigInt::modulo_value = "0"; return ; }
    if (compare(left, right) == -1) { BigInt::division_value = "0"; BigInt::modulo_value = left; return ; }
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();
    string mid_str = left.substr(0, min(left_len, right_len) - 1);
    string rem_str = mid_str;
    int i , j = 0, k, value = -1, ord1, ord2 , right0 = right.at(0) - '0', temp=-1;
    //string res = "";
    char res_temp[MAX_LEN];
    //mid_str = left.substr(0, right_len);
    for(i = 0, k = 0; i < left_len-right_len + 1; i++) {
        mid_str = rem_str + left.at(i + right_len - 1); 
        for (k = 0; k < mid_str.length(); k++) {
            if (mid_str.at(k) != '0' ) break;
        }
        if ( k == mid_str.length()) mid_str = "0";
        else mid_str = mid_str.substr(k, mid_str.length());
        //cout <<"mid_str: " <<mid_str<<endl;
        j = compare(mid_str, right);
        //cout <<"j1 : "<<j<<endl;
        if ( j == 0) {
            rem_str = "";
            res_temp[i] = '1';
        }
        else if ( j == -1) {
            rem_str = mid_str;
            res_temp[i] = '0';
        }
        else {
            if (mid_str.length() > right_len) {
                temp = (mid_str.at(0) - '0') * BASE + (mid_str.at(1) - '0');
            }else{
                temp = mid_str.at(0) - '0';
            }
            //cout<<"temp: "<<temp<<endl;
            ord1 = (int) temp / right0;
            ord2 = (int) temp / (right0 + 1);
            //cout<<"ord1: "<<ord1<<" ord2: "<<ord2<<endl;
            for( k = ord2; k >= ord2 && k <= ord1 && k < BASE; k++) {
                rem_str = pos_sub(mid_str, small_multiply(right, to_string(k))); 
                j = compare(rem_str, right); 
                //cout<<"j2: "<<j<<endl;
                if (j == 0) {
                    rem_str = "" ;
                    res_temp[i] = k + 1 + '0';
                    break;
                }
                else if (j == -1){
                    res_temp[i] = k + '0';
                    break;       
                }
                else{
                
                }
            }
        }
    }
    res_temp[left_len - right_len + 1] = '\0';
    for(i = 0; i < left_len - right_len + 1; i++) {
        if (res_temp[i] != '0') break;
    }
    string res(res_temp + i);
    //cout << "div res: " <<res<<endl;
    if (rem_str.length() == 0) rem_str = "0";
    //cout << "div remain: " <<rem_str<<endl;
    BigInt::division_value = res; BigInt::modulo_value = rem_str;
    return ; 
}

string BigInt::plus(string left, string right) {
    //string res = "";
    char res_temp[MAX_LEN]; 
    //cout<<"plus "<<left<<" "<<right<<endl;
    if (left.length() < right.length()) {
        string temp = left;
        left = right;
        right = temp;
    }
    int left_len = left.length();
    int right_len = right.length();
    res_temp[left_len ] = '\0';
    int ord = 0, mod = 0, i, j;
    for(i = 0; i < right_len; i ++) {
        mod = left.at(left_len - i - 1) - '0' + right.at(right_len - i - 1) - '0' + ord;
        ord = (int)(mod / BASE);
        //res = to_string(mod % BASE) + res;
        res_temp[left_len - i - 1] = mod % BASE + '0';
    }
    
    for (j = i; j < left_len; j ++ ) {
        mod = left.at(left_len - j - 1) - '0' + ord;
        ord = (int)(mod / BASE);
        //res = to_string(mod % BASE) + res;
        res_temp[left_len - j - 1] = mod % BASE + '0';
    }
    string res(res_temp);
    if (ord == 1) {
        res = "1" + res;
    }
        
    //cout <<"plus res "<<res<<endl;
    return res;
}

int BigInt::compare(string left, string right) {
    int left_len = left.length();
    int right_len = right.length();
    if (left_len < right_len) {
        return -1;
    }else if (left_len > right_len) {
        return 1;
    } else{
        for (int i = 0; i < left_len; i++) {
            if (left.at(i) < right.at(i)) {
                return -1;
            }else if (left.at(i) > right.at(i)) {
                return 1;
            }
        }
    }
    return 0; // equality 
}

string BigInt::sub(string left, string right) {
    //cout <<"sub left: "<<left<<" right: "<<right<<endl;
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();

    char left_sign, right_sign;
    left_sign = left.at(0);
    right_sign = right.at(0);
    if (left_sign == '-' && right_sign == '-') {
        return sub(right.substr(1, right_len), left.substr(1, left_len));
    }else if (left_sign != '-' && right_sign == '-') {
        return plus(left, right.substr(1, right_len));
    }else if (left_sign == '-' && right_sign != '-') {
        return "-" + plus(left.substr(1, left_len), right);
    }else {
        int j = compare(left, right);
        if (j == 0) return "0";
        else if (j < 0) { return "-" + pos_sub(right, left); }
        else { return pos_sub(left, right); }
    }
}

// the default sign of result is positive, 
string BigInt::pos_sub(string left, string right) {
    if (compare(left, right) == 0) {return "0"; }
    //string res = "";
    char res_temp[MAX_LEN];
    int left_len = left.length();
    int right_len = right.length();
    //cout << "pos sub: "<<left<<" "<<right<<endl;
    int ord = 0, mod = 0, i, j, left_, right_;
    res_temp[left_len] = '\0';
    for (i = 0; i < right_len; i++) {
        left_ = left.at(left_len - i - 1);  
        right_ = right.at(right_len - i - 1);
        if (left_ - right_ - ord < 0) {
            mod = BASE + left_ - right_ - ord;
            ord = 1;
        }else{
            mod = left_ - right_ - ord;
            ord = 0;
        }
        //res = to_string(mod) + res;
        res_temp[left_len - i - 1] = mod + '0';
    }
    for (j = i; j < left_len; j++) {
        left_ = left.at(left_len - j - 1) - '0';  
        if (left_ - ord < 0) {
            mod = left_ + BASE - ord ;
            ord = 1;
        }else{
            mod = left_ - ord;
            ord = 0;
        }
        res_temp[left_len - j - 1] = mod + '0';
    }
    for (i = 0, j = 0; i < left_len; i++) {
        if (res_temp[i] != '0') {
            j = i;
            break;
        }
    }
    string res(res_temp + j);
    //cout<<"pos sub res 1: "<<res<<endl;
    return res;
}

string BigInt::small_multiply(string left, string right) {
    return small_multiply(left, right, left.length(), right.length());
}

string BigInt::small_multiply(string left, string right, int left_len, int right_len) {
    if (left_len < right_len) {string temp = left; left = right; right = temp; left_len = right_len;}
    //string res_s = "";
    char res_stemp[MAX_LEN];
    res_stemp[left_len] = '\0';
    int i, ord = 0, mod, right_ = right.at(0) - '0' ;
    if (right_ == 0) return "0";
    for (i = 0; i < left_len; i++) {
        mod = (left.at(left_len - i - 1) - '0') * right_ + ord;
        ord = (int)(mod / BASE);
        mod = mod % BASE;
        //res_s = to_string(mod) + res_s;
        res_stemp[left_len-i-1] = mod + '0';
    }
    string res_s(res_stemp);
    if (ord != 0) {
        res_s = to_string(ord) + res_s;
    }
    return res_s;
}

string BigInt::res_to_string(string res, int base) {
    //cout<< "res to string: "<<res + string(base, '0')<<endl;
    if (res.compare("0") == 0) 
        return res;
    else
        return res + string(base, '0');
}
string BigInt::get_substr(string temp, int l_pos, int r_pos) {
    int i ;
    for(i = l_pos; i < r_pos; i++) {
        if (temp.at(i) != '0') {
            return temp.substr(i, r_pos);
        }
    }
    return "0";
}

string BigInt::karatsuba(string left, string right) {
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();
    //cout<<"left_len "<<left_len<<" right_len "<<right_len<<endl;
    //cout <<"1"<<" "<<left<<" "<<right<<endl;
    if (left_len < 2 || right_len < 2) {
        //return to_string(stoi(left, nullptr, BASE) * stoi(right, nullptr, BASE));
        return small_multiply(left, right, left_len, right_len);
    }
    int mid_len = min(left_len, right_len);
    mid_len = (int)(mid_len / 2) ;

    string high1, low1, high2, low2;
 
    high1 = get_substr(left, 0, left_len-mid_len);
    low1 = get_substr(left, left_len - mid_len, left_len);   

    high2 = get_substr(right, 0, right_len-mid_len);
    low2 = get_substr(right, right_len - mid_len, right_len);   

    
    string z0 = karatsuba(low1, low2);
    string z1 = karatsuba(plus(low1, high1), plus(low2, high2));
    string z2 = karatsuba(high1, high2);

    return plus(plus(res_to_string(z2, mid_len*2), res_to_string(pos_sub(pos_sub(z1, z2), z0), mid_len)), z0) ;

}

string BigInt::extended_euclidean(string left, string right) {
    string old_r, r, old_s, s, old_t, t, quotient, prov;
    int left_len, right_len;
    left_len = left.length();
    right_len = right.length();
    old_r = left; r = right;
    old_s = "1"; s = "0";
    old_t = "0"; t = "1";
    while (compare(r, "0") != 0) {
        div(old_r, r);
        quotient = get_division_value();
        //cout <<"quotient: "<<quotient<<endl;
        // (old_r, r) := (r, old_r - quotient * r)
        prov = r;
        r = sub(old_r , multiply(quotient, prov));
        old_r = prov;
        //cout <<"r: "<<r <<" old_r: "<<old_r<<endl;
        // (old_s, s) := (s, old_s - quotient * s)
        prov = s;
        s = sub(old_s , multiply(quotient, prov));
        old_s = prov;
        //cout <<"s: "<<s <<" old_s: "<<old_s<<endl;
        // (old_t, t) := (t, old_t - quotient * t)
        prov = t;
        t = sub(old_t , multiply(quotient, prov));
        old_t = prov;
        //cout <<"t: "<<t <<" old_t: "<<old_t<<endl;
    }
    return old_t;
}

string BigInt::modulo_inverse(string left, string right) {
    string t, newt, r, newr, quotient, prov;
    t = "0"; newt = "1"; r = right; newr = left;
    while (compare(newr, "0") != 0) {
        div(r, newr);
        quotient = get_division_value();
        //cout <<"quotient : "<< quotient<<endl;

        prov = newt;
        newt = sub(t, multiply(quotient, prov));
        t = prov;

        prov = newr;
        newr = sub(r, multiply(quotient, prov));
        r = prov;
    }    
    if (r.compare("1") > 0)
        return "";
    if ( t.compare("0") < 0)
        t = add(t, right);
    return t;
}

string BigInt::_modular_pow(string base, string exponent, string modulus) {
    if (modulus.compare("1") == 0) return "0";
    string result = "1";
    while (compare(exponent, "0")  != 0) {
        modulo(exponent, "2");
        if(get_modulo_value().compare("1") == 0) {
            modulo(multiply(result, base), modulus);
            result = get_modulo_value(); 
        }
        div(exponent, "2");
        exponent = get_division_value();

        modulo(multiply(base, base) , modulus);
        base = get_modulo_value(); 
    }
    return result;
}

void BigInt::_factor(string _number, string _power) {
    _factor_s = "0", _factor_d = "0";
    string _last_factor_d;
    while (true) {
        div(_number, _power);
        _number = get_division_value();
        _last_factor_d = get_modulo_value();
        if (compare(_last_factor_d, "0") == 0) {
            _factor_s = add(_factor_s , "1") ;  
            _factor_d = _number;
        }
        else { break;}
    }
    return;
    //return _s + " " + _d ; 
}

int BigInt::_miller_rabbin(string _prime) {
    int i, k = -1;
    string n_1 = sub(_prime , "1"), x, _base;
    _factor(n_1, "2");
    //cout <<"_factor_s: " << _factor_s << " _factor_d: " << _factor_d << endl;
    for (i = 0; i < sizeof(_bottom_int) / sizeof(_bottom_int[0]); i++) {
        _base = _bottom_int[i];
        //cout <<" bottom int: " << _base << endl;
        x = _modular_pow(_base, _factor_d, _prime) ;
        //cout << "_modular pow: " << x << endl;
        if (compare(x, "1") == 0 || compare(x, n_1) == 0) {
            k = 0; continue;
        }
        _factor_s = pos_sub(_factor_s, "1");
        while (compare(_factor_s, "0") > 0) {
            //cout << "_factor_s: " << _factor_s << endl;
            modulo(multiply(x, x) , _prime);
            x = get_modulo_value();
            if (compare(x, n_1) == 0) {
                k = 0; break;
            }
            _factor_s = pos_sub(_factor_s, "1");
        }
        if (k != 0) return 0;
    }
    return 1; 
}
void BigInt::_get_stored_primes() {
    ifstream infile;
    infile.open(_prime_file_name.data());
    assert(infile.is_open());
    string _prime_line; //string _stored_primes[9592];
    int i = 0 ;
    while (getline(infile, _prime_line, ' ')) {
        if (_prime_line.length() == 0) continue;
        _prime_line.erase(_prime_line.find_last_not_of('\n') + 1, string::npos);
        _stored_primes[i++] = _prime_line;
    }
    infile.close();
    _is_get_stored_primes_ = 1;
    //return _stored_primes;
}

int BigInt::_is_get_stored_primes() {
    return _is_get_stored_primes_;
}

int BigInt::_prime_fast_test(string _prime) {
    for(int l = 1; l < PRIME_AMOUNT; l++) {
    //for(int l = 1; l < 2000; l++) {
        div(_prime, _stored_primes[l]);
        if (get_modulo_value().compare("0") == 0) {
            return 0;
        }
    }
    return 1;
}

string BigInt::generate_prime(int prime_len) {
    string _prime ; int _i = 0;
    if (_is_get_stored_primes() == 0) 
        _get_stored_primes();

    while (true) {
        _i ++;
        if (_i % 100 == 0) {cout << "_i : " << _i << endl;}
        _prime = _generate_prime(prime_len);
        //while (_prime_fast_test(_prime) == 0) {
        //    _prime = plus(_prime , "2");
        //}
        if (_prime_fast_test(_prime) == 0) {
            continue;
        }
        if (_miller_rabbin(_prime) == 1) {
            cout << "_i : " << _i << endl;
            return _prime;
        }
    }
}

string BigInt::_generate_prime(int prime_len) {
    char prime[prime_len + 1];
    prime[prime_len] = '\0' ;
    int i;
    for (i = 0; i < prime_len ; i++) {
        if (i == 0) {
            prime[i] = (int) rand() % (BASE - 1) + 1 + '0';
        } else if (i == prime_len - 1) {
            prime[i] = ((int) rand() % ((int) BASE/2) + 1) * 2 - 1 + '0';
        }
        else{
            prime[i] = (int) rand() % BASE + '0';
        }
    }
    return prime;
}


string BigInt::_dec2hex(unsigned int _temp, int _len) {
    
    stringstream ioss; string s_temp;
    //ioss << setiosflags(ios::uppercase) << hex << _temp;
    ioss << hex << _temp;
    ioss >> s_temp;
    if (s_temp.length() < _len) {
        string _zero("0", _len - s_temp.length());
        s_temp = _zero + s_temp;
    }
    return s_temp;
    //char ch_temp[10];
    //sprintf_s(ch_temp, "%8X", _temp);
    
    //return string(ch_temp);
}

string BigInt::_hex2dec(string _temp) {

    /**
    unsigned int x;
    stringstream ss;
    ss << hex << _temp;
    ss >> x;
    return x; */
    string _res = "";
    for (int i = 0; i < _temp.length(); i++) {
        _res += to_string((int)_temp.at(i));
    }
    return _res;
}


