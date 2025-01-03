// does not starts with smallest value of x as
// suggested in algorithm
#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<math.h>
#include<time.h>
# define RANGE 1000
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
    clock_t time1,time2;
    gmp_randstate_t state;
    gmp_randinit_default(state);
    int m;long i;int ret;
    mpz_t exp;
    mpz_init(exp);
    struct Point B,R;
    mpz_init(EC.p);
    mpz_init(EC.b);
    mpz_init(EC.a);
    mpz_init(B.x);
    mpz_init(B.y);
    mpz_init(R.x);
    mpz_init(R.y);
    mpz_t y,p,t,n;
    mpz_init(y);
    mpz_init(p);
    mpz_init(t);
    mpz_init(n);
    mpz_t array[5];
    for(i=0;i<5;i++)
        mpz_init(array[i]);
    int P[5]={1,6,24,36,36};
    scanf("%d",&m);
    mpz_t x,two;
    mpz_init(x);
    mpz_init_set_ui(two,2);
    time1=clock();
    mpz_pow_ui(x,two,m/4);
    mpz_t negx,temp;
    mpz_init(negx);mpz_init(temp);
    mpz_neg(negx,x);
    evaluate(P,4,negx,temp,array);
    i=1;
    mpz_t x1;mpz_init(x1);
    mpz_t x2;mpz_init(x2);
    mpz_pow_ui(x1,two,m/4);
    while(mpz_sizeinbase(temp,2)>=m)
    {
        mpz_pow_ui(x2,two,m/4-i);
        i++;
        mpz_neg(negx,x2);
        evaluate(P,4,negx,temp,array);
    }
    while(mpz_sizeinbase(temp,2)!=m)
    {
        mpz_add(x,x2,x1);
        mpz_div_ui(x,x,2);
        // gmp_printf("\n m=%d temp=%d temp=%Zd \n", m,mpz_sizeinbase(temp,2),temp);
        mpz_neg(negx,x);
        evaluate(P,4,negx,temp,array);
        if(mpz_sizeinbase(temp,2)<m)
            mpz_set(x2,x);
        else
            mpz_set(x1,x);
    }
    int ctr=0;
    while(mpz_sizeinbase(temp,2)==m)
    {
        ctr++;
        mpz_sub_ui(x,x,1);
        mpz_neg(negx,x);
        evaluate(P,4,negx,temp,array);
        if(ctr>RANGE)
            break;
    }
    mpz_add_ui(x,x,1);
    mpz_neg(negx,x);
    evaluate(P,4,negx,temp,array);
    fflush(stdout);
    ctr=0;
    while(1)
    {
        ctr++;
        mpz_mul(t,x,x);
        mpz_mul_ui(t,t,6);
        mpz_add_ui(t,t,1);
        evaluate(P,4,negx,p,array);
        mpz_add_ui(n,p,1);
        mpz_sub(n,n,t);
        if(mpz_probab_prime_p(p,1000) &&
                mpz_probab_prime_p(n,1000))
            break;
        evaluate_neg(p,array);
        mpz_add_ui(n,p,1);
        mpz_sub(n,n,t);
        if(mpz_probab_prime_p(p,1000) &&
                mpz_probab_prime_p(n,1000))
            break;
        mpz_add_ui(x,x,1);
        mpz_neg(negx,x);
    }
    mpz_set(EC.p,p);
    mpz_set_ui(EC.a,0);
    mpz_set_ui(EC.b,0);
    do
    {
        do
        {
            mpz_add_ui(EC.b,EC.b,1);
            mpz_add_ui(temp,EC.b,1);
        }
        while(mpz_legendre(temp,EC.p)!=1);
        ret=root(temp,EC.p,&y,1);
        // gmp_printf("\n temp=%Zd y=%Zd
        EC.b=%Zd\n", temp,y,EC.b);
        mpz_set_ui(B.x,1);
        fflush(stdout);
        mpz_set(B.y,y);
        mpz_set_ui(R.x,1);
        mpz_set(R.y,y);
        mpz_set(exp,n);
        Scalar_Multiplication(B,&R,exp);
        if(mpz_cmp_ui(R.x,0)==0 &&
                mpz_cmp_ui(R.y,0)==0)
            ret=1;
        else
            ret=0;
    }
    while(ret!=1);
    time2=clock();
    gmp_printf("\n p=%Zd t=%Zd n=%Zd B.x=%Zd
            B.y=%Zd y=%Zd b=%Zd\n",EC.p,t,n,B.x,B.y,y,EC.b);
    printf("\n Time Taken is %f \n",(float) (time2-
                time1)/CLOCKS_PER_SEC);
    FILE *fp;
    fp=fopen("Domain_Parameters","w");
    gmp_fprintf(fp,"%Zd\n",EC.p);
    gmp_fprintf(fp,"0\n");
    gmp_fprintf(fp,"%Zd\n",EC.b);
    gmp_fprintf(fp,"%Zd\n",B.x);
    gmp_fprintf(fp,"%Zd\n",B.y);
    gmp_fprintf(fp,"%Zd\n",n);
    fclose(fp);
}
Point_Addition(struct Point P,struct Point Q,
        struct Point *R)
{
    mpz_t temp,slope;
    mpz_init(temp);
    mpz_init(slope);
    if(mpz_cmp_ui(P.x,0)==0 && mpz_cmp_ui(P.y,0)==0)
    { mpz_set(R->x,Q.x); mpz_set(R->y,Q.y);
        mpz_clear(temp);mpz_clear(slope);return;}
    if(mpz_cmp_ui(Q.x,0)==0 && mpz_cmp_ui(Q.y,0)==0)
    { mpz_set(R->x,P.x); mpz_set(R->y,P.y);
        mpz_clear(temp);mpz_clear(slope);return;}
    if(mpz_cmp_ui(Q.y,0)!=0)
    {mpz_sub(temp,EC.p,Q.y);mpz_mod(temp,temp,
            EC.p);}
    else
        mpz_set_ui(temp,0);
    if(mpz_cmp(P.y,temp)==0 &&
            mpz_cmp(P.x,Q.x)==0)
    { mpz_set_ui(R->x,0); mpz_set_ui(R->y,0);
        mpz_clear(temp);mpz_clear(slope);return;}
    if(mpz_cmp(P.x,Q.x)==0 && mpz_cmp(P.y,Q.y)==0)
    {Point_Doubling(P,R);mpz_clear(temp);mpz_clear(sl
            ope);return;}
    else
    {
        mpz_sub(temp,P.x,Q.x);
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
        mpz_clear(temp);
        mpz_clear(slope);
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
    mpz_clear(temp);
    mpz_clear(slope);
}
Scalar_Multiplication(struct Point P,struct Point
        *R,mpz_t m)
{
    struct Point Q,T;
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
        mpz_set(Q.x,T.x);
        mpz_set(Q.y,T.y);
        mpz_set(T.x,R->x);
        mpz_set(T.y,R->y);
        if(mpz_tstbit(m,loop))
            Point_Addition(T,Q,R);
    } }
root(mpz_t x,mpz_t p,mpz_t *s,int n)
{
    int i, ret;
    for(i=1;i<=n;i++)
    {
        if(mpz_cmp_ui(p,2)==0)
            ret=tworoot(x,p,s,i);
        else
            ret=sqroot(x,p,s,i);
    }
    return(ret);
}
sqroot(mpz_t r,mpz_t p,mpz_t *s,int n)
{
    int i,ret;
    mpz_t f,pn,op,po,pn1,ch,x;
    mpz_init(f);
    mpz_init(x);
    mpz_init(ch);
    mpz_init(pn);
    mpz_init(op);
    mpz_init(po);
    mpz_init(pn1);
    mpz_pow_ui(pn,p,n);
    mpz_mod(x,r,pn);
    if(mpz_legendre(x,p)==-1)
        return(0);
    if(n==1)
    {
        mpz_sub_ui(ch,p,3);
        if(mpz_divisible_ui_p(ch,4)!=0)
            case1(x,p,s);
        else
        {
            mpz_sub_ui(ch,p,5);
            if(mpz_divisible_ui_p(ch,8)!=0)
            {
                ret=case2(x,p,s);
                if(ret!=1)
                    squareroot(x,p,s);
            }
            else
                squareroot(x,p,s);
        } }
    else
    {
        mpz_pow_ui(pn,p,n-1);
        mpz_mul_ui(f,s[0],2);
        mpz_invert(f,f,pn);
        mpz_mul(op,s[0],s[0]);
        mpz_sub(po,x,op);
        mpz_cdiv_q(po,po,pn);
        mpz_mul(po,po,f);
        mpz_mod(po,po,p);
        mpz_mul(op,po,pn);
        mpz_add(s[0],s[0],op);
        mpz_pow_ui(pn1,p,n);
        mpz_mod(s[0],s[0],pn1);
    }
    mpz_clear(f);
    mpz_clear(x);
    mpz_clear(ch);
    mpz_clear(pn);
    mpz_clear(op);
    mpz_clear(po);
    mpz_clear(pn1);
    return(1);
}
squareroot(mpz_t a,mpz_t p,mpz_t *s)
{
    mpz_t i,r,j,e,d,c,b,w,t;
    mpz_init(r);
    mpz_init(i);
    mpz_init(j);
    mpz_init(e);
    mpz_init(d);
    mpz_init(b);
    mpz_init(w);
    mpz_init(t);
    mpz_init(c);
    mpz_set_ui(b,1);
    while(mpz_legendre(b,p)!=-1)
        mpz_add_ui(b,b,1);
    mpz_sub_ui(t,p,1);
    mpz_set_ui(w,0);
    mpz_mod_ui(j,t,2);
    while(mpz_cmp_ui(j,0) ==0)
    {
        mpz_add_ui(w,w,1);
        mpz_div_ui(t,t,2);
        mpz_mod_ui(j,t,2);
    }
    mpz_invert(i,a,p);
    mpz_powm(c,b,t,p);
    mpz_add_ui(t,t,1);
    mpz_cdiv_q_ui(t,t,2);
    mpz_powm(r,a,t,p);
    while(mpz_cmp(j,w)<0)
    {
        mpz_sub(e,w,j);
        mpz_sub_ui(e,e,1);
        mpz_set_ui(t,2);
        mpz_powm(e,t,e,p);
        mpz_mul(b,r,r);
        mpz_mul(d,b,i);
        mpz_powm(d,d,e,p);
        mpz_add_ui(d,d,1);
        if(mpz_divisible_p(d,p)!=0)
        {
            mpz_mul(r,r,c);
            mpz_mod(r,r,p);
        }
        mpz_mul(c,c,c);
        mpz_mod(c,c,p);
        mpz_add_ui(j,j,1);
    }
    mpz_set(s[0],r);
    mpz_clear(r);
    mpz_clear(i);
    mpz_clear(j);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(b);
    mpz_clear(w);
    mpz_clear(t);
    mpz_clear(c);
    return;
}
case1(mpz_t a,mpz_t p,mpz_t *s)
{
    mpz_t x,r;
    mpz_init(r);
    mpz_init(x);
    mpz_add_ui(x,p,1);
    mpz_div_ui(x,x,4);
    mpz_powm(r,a,x,p);
    mpz_set(s[0],r);
    mpz_clear(r);
    mpz_clear(x);
    return;
}
case2(mpz_t a,mpz_t p,mpz_t *s)
{
    mpz_t x,b,c,r,d;
    mpz_init(r);
    mpz_init(b);
    mpz_init(c);
    mpz_init(x);
    mpz_init(d);
    mpz_sub_ui(x,p,1);
    mpz_div_ui(x,x,4);
    mpz_powm(d,a,x,p);
    if(mpz_cmp_ui(d,1)==0)
    {
        mpz_add_ui(x,p,3);
        mpz_div_ui(x,x,8);
        mpz_powm(r,a,x,p);
        mpz_set(s[0],r);
        mpz_clear(r);
        mpz_clear(b);
        mpz_clear(c);
        mpz_clear(x);
        mpz_clear(d);
        return(1);
    }
    mpz_sub_ui(x,p,1);
    if(mpz_cmp(d,x)==0)
    {
        mpz_mul_ui(b,a,4);
        mpz_sub_ui(x,p,5);
        mpz_div_ui(x,x,8);
        mpz_powm(b,b,x,p);
        mpz_mul_ui(c,a,2);
        mpz_mul(r,c,b);
        mpz_mod(r,r,p);
        mpz_set(s[0],r);
        mpz_clear(r);
        mpz_clear(b);
        mpz_clear(c);
        mpz_clear(x);
        mpz_clear(d);
        return(1);
    }
    mpz_clear(r);
    mpz_clear(b);
    mpz_clear(c);
    mpz_clear(x);
    mpz_clear(d);
    return(0);
}
tworoot(mpz_t x,mpz_t p,mpz_t *s,int n)
{
    mpz_t pn;
    mpz_t pr,pr1;
    mpz_init(pn);
    mpz_init(pr1);
    mpz_init(pr);
    mpz_pow_ui(pn,p,n);
    if(n==1 || n==2 || n==3)
    {
        if(n==1)
        {
            mpz_mod_ui(pr1,x,2);
            if(mpz_cmp_ui(pr1,0)==0)
                return(0);
            else
            {
                mpz_set_ui(s[0],1);
                mpz_clear(pn);
                mpz_clear(pr1);
                mpz_clear(pr);
                return(1);
            } }
        if(n==2)
        {
            mpz_mod_ui(pr1,x,2);
            if(mpz_cmp_ui(pr1,0)==0)
            {
                mpz_clear(pn);
                mpz_clear(pr1);
                mpz_clear(pr);
                return(0);
            }
            else
            {
                mpz_sub_ui(pr1,x,1);
                mpz_mod_ui(pr1,x,4);
                if(mpz_cmp_ui(pr1,0)==0)
                {
                    mpz_set_ui(s[0],1);
                    {
                        mpz_clear(pn);
                        mpz_clear(pr1);
                        mpz_clear(pr);
                        return(1);
                    } }
                else
                {
                    mpz_clear(pn);
                    mpz_clear(pr1);
                    mpz_clear(pr);
                    return(0);
                } } }
        if(n==3)
        {
            mpz_sub_ui(pr1,x,1);
            mpz_mod_ui(pr1,pr1,8);
            if(mpz_cmp_ui(pr1,0)!=0)
            {
                mpz_clear(pn);
                mpz_clear(pr1);
                mpz_clear(pr);
                return(0);
            }
            else
            {
                mpz_set_ui(s[0],1);
                mpz_clear(pn);
                mpz_clear(pr1);
                mpz_clear(pr);
                return(1);
            } } }
    else
    {
        mpz_sub_ui(pr1,x,1);
        mpz_mod_ui(pr1,pr1,8);
        if(mpz_cmp_ui(pr1,0)!=0)
        {
            mpz_clear(pn);
            mpz_clear(pr1);
            mpz_clear(pr);
            return(0);
        }
        mpz_pow_ui(pr,s[0],3);
        mpz_sub_ui(pr1,x,2);
        mpz_mul(pr1,pr1,s[0]);
        mpz_sub(s[0],pr,pr1);
        mpz_div_ui(s[0],s[0],2);
        mpz_mod(s[0],s[0],pn);
        mpz_clear(pn);
        mpz_clear(pr1);
        mpz_clear(pr);
        return(1);
    } }
evaluate(int *Poly1, int Degree_K, mpz_t val,mpz_t
        eval,mpz_t *array)
{
    int i;
    mpz_t tt;
    mpz_init_set_ui(tt,1);
    mpz_set_ui(eval,Poly1[0]);
    mpz_set_ui(array[0],1);
    for(i=1;i<=Degree_K;i++)
    {
        mpz_mul(tt,tt,val);
        mpz_mul_ui(array[i],tt,Poly1[i]);
        mpz_add(eval,eval,array[i]);
    }
    mpz_clear(tt);
    return;
}
evaluate_neg(mpz_t eval,mpz_t *array)
{
    mpz_set_ui(eval,0);
    int i;
    for(i=0;i<5;i++)
    {
        if(i%2==0)
            mpz_add(eval,eval,array[i]);
        else
            mpz_sub(eval,eval,array[i]);
    } }
evaluate_dec(mpz_t eval,mpz_t *array)
{
    mpz_t temp;mpz_init(temp);
    mpz_add_ui(eval,array[0],102);
    mpz_mul_ui(temp,array[1],51);
    mpz_add(eval,eval,temp);
    mpz_mul_ui(temp,array[2],29);
    mpz_div_ui(temp,temp,2);
    mpz_add(eval,eval,temp);
    mpz_mul_ui(temp,array[3],5);
    mpz_add(eval,eval,temp);
    mpz_add(eval,eval,array[4]);
    mpz_clear(temp);
}
