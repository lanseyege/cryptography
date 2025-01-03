#include <iostream>
//#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/sm3.h>
#include <cryptopp/sm4.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using namespace std;
using namespace CryptoPP;

int main() {
    SHA256 hash;
    string message = "abcdefghijklmnopqrstuvwxyz";
    string digest;
    StringSource s(message, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    cout << "sha256: " << digest << endl;
    SM3 sm3;
    digest = "";
    StringSource s2(message, true, new HashFilter(sm3, new HexEncoder(new StringSink(digest))));
    cout << "sm3: " << digest << endl;

    SM4 sm4;
    digest = "";
    StringSource s3(message, true, new HashFilter(sm4, new HexEncoder(new StringSink(digest))));
    cout << "sm4: " << digest << endl;


}
