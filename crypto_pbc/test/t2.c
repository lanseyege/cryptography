#include <gmp.h>

int main() {
    mpz_t t1 ; mpz_init(t1);
    const char * _t1 =  "0x93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD"; //"0xB640000002A3A6F1D603AB4FF58EC74521F2934B1A7AEEDBE56F9B27E351457D";
    mpz_set_str(t1, _t1, 0);
    gmp_printf("t1: %Zd\n", t1);
    const char * _t2 =  "0x21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616";//"0xB640000002A3A6F1D603AB4FF58EC74449F2934B18EA8BEEE56EE19CD69ECF25";
    _t2 = "54501653413638164902196427291530872607918359762429484789843825195912643777279";
    _t2 = "70945093796437916589482962664195396011862017988090975794354720681487709519670";
    mpz_set_str(t1, _t2, 0);
    gmp_printf("t1: %Zx\n", t1);
}