#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

int main() {
    string fname = "../data/primes-100k.dat";
    ifstream infile;
    infile.open(fname.data());
    assert(infile.is_open());
    string _prime_line;
    char ch; int i = 0;
    string stored[9592];
    while (getline(infile, _prime_line, ' ')) {
    //while (get_line(infile, _prime_line)) {
        //string temp[] = _prime_line.Split(" ".ToCharArray());
        //cout << _prime_line << endl;
        //string temp[] = _prime_line.Split(" ");
        //for(int i = 0; i < (int)sizeof(temp) / sizeof(temp[0]); i++) {
        //    cout << temp[i] << endl;
        //}
        //if (_prime_line.compare(" ") == 0) continue;
        //if (isspace(_prime_line)) {continue;}
        if (_prime_line.length()  == 0) continue;
        //cout << temp[0] <<endl;
        _prime_line.erase(_prime_line.find_last_not_of('\n') + 1, string::npos) ;
        //if ( i == 10) {
            //cout << _prime_line << endl;
            //break; }
        stored[i++] = _prime_line;
        cout << _prime_line << endl;
    }
    infile.close();
    //cout << i << endl;
    int n = sizeof(stored) / sizeof(stored[0]);
    //cout << n << endl;
    //cout <<stored[0] << endl;

}
