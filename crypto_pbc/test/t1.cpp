#include <iostream>
#include <gmp.h>

using namespace std;

int main() {
    mpz_t p; mpz_init(p);
    mpz_t x1; mpz_init(x1);
    mpz_t x2; mpz_init(x2);
    mpz_t y1; mpz_init(y1);
    mpz_t y2; mpz_init(y2);
    mpz_t b; mpz_init(b);
    mpz_t pk; mpz_init(pk);

    mpz_set_str(p, "82434016654578246444830763105245969129603161266935169637912592173415460324733", 10);
    mpz_set_str(x1, "93DE051D62BF718FF5ED0704487D01D6E1E4086909DC3280E8C4E4817C66DDDD", 16);
    mpz_set_str(y1, "21FE8DDA4F21E607631065125C395BBC1C1C00CBFA6024350C464CD70A3EA616", 16);
    //mpz_set_str(x2,"85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141", 16);//3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B", 16);
    mpz_set_str(x2,"85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D88061413722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B", 16);
    //mpz_set_str(y2,"17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96", 16);//A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7", 16);
    mpz_set_str(y2,"17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7", 16);
    mpz_set_str(b, "10", 10);

    mpz_t left; mpz_init(left);
    mpz_t right; mpz_init(right);
    mpz_pow_ui(pk, p, 1);
    //mpz_mul_ui(pk, p, 2);
    mpz_mul(left, y2, y2);
    gmp_printf("left %Zx\n", left);
    mpz_mod(left, left, pk);
    mpz_pow_ui(right, x2, 3);
    mpz_sub(right, right, b);
    gmp_printf("right %Zx\n", right);
    mpz_mod(right, right, pk);
    gmp_printf("left  %Zx\n", left);
    gmp_printf("right %Zx\n", right);

    mpz_mul(left, y1, y1);
    mpz_mod(left, left, p);
    mpz_pow_ui(right, x1, 3);
    mpz_add(right, right, b);
    mpz_mod(right, right, p);

    gmp_printf("left  %Zx\n", left);
    gmp_printf("right %Zx\n", right);

    mpz_set_str(x1, "94736ACD2C8C8796CC4785E938301A139A059D3537B6414140B2D31EECF41683115BAE85F5D8BC6C3DBD9E5342979ACCCF3C2F4F28420B1CB4F8C0B59A19B158", 16);
    //mpz_set_str(x1, "787ED7B8A51F3AB84E0A66003F32DA5C720B17ECA7137D39ABC66E3C80A892FF", 16);
    mpz_set_str(y1, "7AA5E47570DA7600CD760A0CF7BEAF71C447F3844753FE74FA7BA92CA7D3B55F27538A62E7F7BFB51DCE08704796D94C9D56734F119EA44732B50E31CDEB75C1", 16);
    //mpz_set_str(y1, "769DE61791E5ADC4B9FF85A31354900B202871279A8C49DC3F220F644C57A7B1", 16);
    mpz_mul(left, y1, y1);
    mpz_mod(left, left, p);
    mpz_pow_ui(right, x1, 3);
    mpz_add(right, right, b);
    mpz_mod(right, right, p);

    gmp_printf("left  %Zx\n", left);
    gmp_printf("right %Zx\n", right);

    int a = mpz_cmp(left, right);
    cout << a << endl;
    mpz_t N; mpz_init(N);
    mpz_t temp; mpz_init(temp);
    mpz_t temp2; mpz_init(temp2);
    mpz_set_str(N, "B640000002A3A6F1D603AB4FF58EC74449F2934B18EA8BEEE56EE19CD69ECF25", 16);
    mpz_pow_ui(temp, p, 12);
    mpz_sub_ui(temp, temp, 1);
    mpz_mod(temp2 , temp, N);
    gmp_printf("mod %Zx\n",temp2);

    mpz_t zx1 ; mpz_init(zx1);
    mpz_t zy1 ; mpz_init(zy1);
    mpz_t zx2 ; mpz_init(zx2);
    mpz_t zy2 ; mpz_init(zy2);
    mpz_set_str(zx1, "85AEF3D078640C98597B6027B441A01FF1DD2C190F5E93C454806C11D8806141", 16);
    mpz_set_str(zy1, "3722755292130B08D2AAB97FD34EC120EE265948D19C17ABF9B7213BAF82D65B", 16);
    mpz_set_str(zx2, "17509B092E845C1266BA0D262CBEE6ED0736A96FA347C8BD856DC76B84EBEB96", 16);
    mpz_set_str(zy2, "A7CF28D519BE3DA65F3170153D278FF247EFBA98A71A08116215BBA5C999A7C7", 16);

    mpz_mul(left, zy1, zy1);
    mpz_mul(temp, zx1, zx1);
    mpz_sub(left, left, temp);
    mpz_mod(left, left, pk);

    gmp_printf("left %Zx\n", left);

    mpz_pow_ui(right, zy2, 3);
    mpz_mul(temp, zy2, zy2);
    mpz_mul(temp, temp, zx2);
    mpz_sub(right, right, temp);
    mpz_mul(temp, zx2, zx2);
    mpz_mul(temp, temp, zy2);
    mpz_mul_ui(temp, temp, 2);
    mpz_sub(right, right, temp);
    mpz_mod(right, right, pk);

    gmp_printf("right %Zx\n", right);
}
