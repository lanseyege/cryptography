// Point at Infinity is Denoted by (0,0)
#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
struct Elliptic_Curve
{
    mpz_t a;
    mpz_t b;
    mpz_t p;
};
struct Point
{
    mpz_t x;
    mpz_t y;
};
struct Elliptic_Curve EC;
main()
{
    int choice;
    mpz_init(EC.a); mpz_init(EC.b); mpz_init(EC.p);
    Select_EC();
    printf("\n Enter Choice of Operation\n");
    printf("\n Enter 1 For Point Addition Operation\n");
    printf("\n Enter 2 For Scalar Multiplication
            Operation\n");
    scanf("%d",&choice);
    struct Point P,R;
    mpz_init(P.x); mpz_init(P.y);
    mpz_init_set_ui(R.x,0); mpz_init_set_ui(R.y,0);
    printf("\n Enter Points P(x,y) and/or Q(x,y) to be
            Added\n");
    gmp_scanf("%Zd",&P.x);
    gmp_scanf("%Zd",&P.y);
    if(choice==1)
    {
        struct Point Q;
        mpz_init(Q.x); mpz_init(Q.y);
        gmp_scanf("%Zd",&Q.x);
        gmp_scanf("%Zd",&Q.y);
        Point_Addition(P,Q,&R);
    }
    else
    {
        printf("\n Enter m to Find mP\n");
        mpz_t m;
        mpz_init(m);
        gmp_scanf("%Zd",&m);
        Scalar_Multiplication(P,&R,m);
    }
    gmp_printf("\n Resultant Point is %Zd,
            %Zd",R.x,R.y);
}
Select_EC()
{
    printf("\n Enter Elliptic Curve Parameters i.e. a,b and
            p");
    gmp_scanf("%Zd",&EC.a);
    gmp_scanf("%Zd",&EC.b);
    gmp_scanf("%Zd",&EC.p);
}
Point_Addition(struct Point P,struct Point Q,struct Point
        *R)
{
    mpz_mod(P.x,P.x,EC.p);
    mpz_mod(P.y,P.y,EC.p);
    mpz_mod(Q.x,Q.x,EC.p);
    mpz_mod(Q.y,Q.y,EC.p);
    mpz_t temp,slope;
    mpz_init(temp);
    mpz_init_set_ui(slope,0);
    if(mpz_cmp_ui(P.x,0)==0 &&
            mpz_cmp_ui(P.y,0)==0)
    { mpz_set(R->x,Q.x); mpz_set(R->y,Q.y); return;}
    if(mpz_cmp_ui(Q.x,0)==0 &&
            mpz_cmp_ui(Q.y,0)==0)
    { mpz_set(R->x,P.x); mpz_set(R->y,P.y);
        return;}
    if(mpz_cmp_ui(Q.y,0)!=0)
    {mpz_sub(temp,EC.p,Q.y);mpz_mod(temp,temp,EC
            .p);}
    else
        mpz_set_ui(temp,0);
    // gmp_printf("\n temp=%Zd\n",temp);
    if(mpz_cmp(P.y,temp)==0 &&
            mpz_cmp(P.x,Q.x)==0)
    { mpz_set_ui(R->x,0); mpz_set_ui(R->y,0); return;}
    if(mpz_cmp(P.x,Q.x)==0 &&
            mpz_cmp(P.y,Q.y)==0)
    {
        Point_Doubling(P,R);
        return;
    }
    else
    {
        mpz_sub(temp,P.x,Q.x);
        mpz_mod(temp,temp,EC.p);
        mpz_invert(temp,temp,EC.p);
        mpz_sub(slope,P.y,Q.y);
        mpz_mul(slope,slope,temp);
        mpz_mod(slope,slope,EC.p);
        mpz_mul(R->x,slope,slope);
        mpz_sub(R->x,R->x,P.x);
        mpz_sub(R->x,R->x,Q.x);
        mpz_mod(R->x,R->x,EC.p);
        mpz_sub(temp,P.x,R->x);
        mpz_mul(R->y,slope,temp);
        mpz_sub(R->y,R->y,P.y);
        mpz_mod(R->y,R->y,EC.p);
        return;
    } }
Point_Doubling(struct Point P,struct Point *R)
{
    mpz_t slope,temp;
    mpz_init(temp);
    mpz_init(slope);
    if(mpz_cmp_ui(P.y,0)!=0)
    {
        mpz_mul_ui(temp,P.y,2);
        mpz_invert(temp,temp,EC.p);
        mpz_mul(slope,P.x,P.x);
        mpz_mul_ui(slope,slope,3);
        mpz_add(slope,slope,EC.a);
        mpz_mul(slope,slope,temp);
        mpz_mod(slope,slope,EC.p);
        mpz_mul(R->x,slope,slope);
        mpz_sub(R->x,R->x,P.x);
        mpz_sub(R->x,R->x,P.x);
        mpz_mod(R->x,R->x,EC.p);
        mpz_sub(temp,P.x,R->x);
        mpz_mul(R->y,slope,temp);
        mpz_sub(R->y,R->y,P.y);
        mpz_mod(R->y,R->y,EC.p);
    }
    else
    {
        mpz_set_ui(R->x,0);
        mpz_set_ui(R->y,0);
    } 
}
Scalar_Multiplication(struct Point P,struct Point *R,mpz_t
        m)
{ struct Point Q,T;
    mpz_init(Q.x); mpz_init(Q.y);
    mpz_init(T.x); mpz_init(T.y);
    long no_of_bits,loop;
    no_of_bits=mpz_sizeinbase(m,2);
    mpz_set_ui(R->x,0);mpz_set_ui(R->y,0);
    if(mpz_cmp_ui(m,0)==0)
        return;
    mpz_set(Q.x,P.x);
    mpz_set(Q.y,P.y);
    if(mpz_tstbit(m,0)==1)
    {mpz_set(R->x,P.x);mpz_set(R->y,P.y);}
    for(loop=1;loop<no_of_bits;loop++)
    {
        mpz_set_ui(T.x,0);
        mpz_set_ui(T.y,0);
        Point_Doubling(Q,&T);
        gmp_printf("\n %Zd %Zd %Zd %Zd ",Q.x,Q.y,T.x,T.y);
        mpz_set(Q.x,T.x);
        mpz_set(Q.y,T.y);
        mpz_set(T.x,R->x);
        mpz_set(T.y,R->y);
        if(mpz_tstbit(m,loop))
            Point_Addition(T,Q,R);
    } 
}
