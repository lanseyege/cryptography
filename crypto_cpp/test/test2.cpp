#include <iostream>
#include <fstream>

#include "bigint.h"

using namespace std;




int main() {

    BigInt bigint;
    //string left = "12344556677888901881821873658767234978";
    //string right = "12344556677888901881821997349774096836385";
    //string left = "123456";
    //string right = "456789";
    //string left = "444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444445";
    //string right = "67777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777770011223445677777";    
    ifstream infile;
    infile.open("co-prime.dat");
    string left, right;
    infile>>left;
    infile>>right;
    infile.close();

    //string res = bigint.karatsuba(left, right);
    string res = bigint.multiply(left, right);
    cout<<res<<endl;
    cout<<"left: "<<left<<endl;
    cout<<"right: "<<right<<endl;
}
