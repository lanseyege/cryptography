#include <iostream>
#include <sstream>

using namespace std;



int main() {
    string str = "wekrwjeroueo234nldsk";
    cout << "str: " << str << endl;
    string result = "";
    string temp;
    stringstream ss;
    for (int i = 0; i < str.size(); i++) {
        ss << hex << int(str[i]) << endl;
        ss >> temp;
        result += temp;
    }
    cout << "result: " << result << endl;

    string res2;
    for (int i = 0; i < result.length(); i += 2 ) {
        string byte = result.substr(i, 2);
        char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
        res2.push_back(chr);
    }
    cout << "res2: " << res2 << endl;
}
