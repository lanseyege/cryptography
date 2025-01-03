#include <iostream>
#include <fstream>

#include "bigint.h"

using namespace std;




int main() {

    BigInt bigint;
    ifstream infile;
    infile.open("div-test.dat");
    string left, right;
    infile>>left;
    infile>>right;
    infile.close();

    cout<<"left: "<<left<<endl;
    cout<<"right: "<<right<<endl;

    bigint.division(left, right);
    string res = bigint.get_division_value();
    string modulo = bigint.get_modulo_value();
    cout<<"res: "<<res<<endl;
    cout<<"modulo: "<<modulo<<endl;
}
