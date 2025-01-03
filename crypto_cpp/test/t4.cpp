#include <iostream>
#include <fstream>
using namespace std;

int main() {


    ifstream infile;
    infile.open("../co-prime.dat");
    string left, right;
    infile>>left;
    infile>>right;
    infile.close();
    cout<<left<<endl;
    cout<<right<<endl;
}
