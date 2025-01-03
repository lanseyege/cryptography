#include <iostream>
#include <fstream>

#include "bigint.h"

using namespace std;




int main() {

    BigInt bigint;
    ifstream infile;
    infile.open("extend-eclid.dat");
    string left, right;
    infile>>left;
    infile>>right;
    infile.close();

    //string res = bigint.extended_euclidean(left, right);
    string res = bigint.modulo_inverse(left, right);
    cout<<"left: "<<left<<endl;
    cout<<"right: "<<right<<endl;

    cout<<"res: "<<res<<endl;
}
