#include <pbc/pbc.h>

int main()  {

    pairing_t pairing;
    char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    
    element_t g, h;
    element_t public_key, secret_key;
    element_t sig;
    element_t temp1, temp2;
    element_init_G2(g, pairing);
    element_init_G2(public_key, pairing);
    element_init_G1(h, pairing);
    element_init_G1(sig, pairing);
    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);
    element_init_Zr(secret_key, pairing);

    element_printf("g %B\n", g);

    element_random(g);
    element_random(secret_key);
    element_pow_zn(public_key, g, secret_key);

}
