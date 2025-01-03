#include <iostream>

using namespace std;

void func(string left, string right) {
    int left_len = left.length();
    int right_len = right.length();
    if (left_len < right_len) {string temp = left; left=right; right = temp;}
}

int main() {
    string left = "abcd";
    string right = "dergh";
    
    cout <<left<<" " <<right<<endl;
    func(left, right);
    cout <<left<<" " <<right<<endl;
}
