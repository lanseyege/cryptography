// Point at Infinity is Denoted by (0,0)
#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<time.h>
main()
{
    clock_t time1,time2;double Total_Time;
    int choice;
    mpz_t p,g;
    mpz_init(p);
    mpz_init(g);
    FILE *fileptr;
    fileptr=fopen("Domain_Parameters_Zp","r");
    gmp_fscanf(fileptr,"%Zd",&p);
    gmp_fscanf(fileptr,"%Zd",&g);
    fclose(fileptr);
    // printf("\n Enter Your Choice\n");
    // printf("\n Enter 1 to Encrypt\n");
    // printf("\n Enter 2 to Decrypt\n");
    // scanf("%d",&choice);
    long Iterations,i;
    printf("\n Enter Number Of Iterations\n");
    scanf("%ld",&Iterations);
    time1=clock();
    for(i=0;i<Iterations;i++)
    {
        // if(choice==1)
        Encrypt(p,g);
        // else
        Decrypt(p,g);
    }
    time2=clock();
    Total_Time=(double)(time2-time1)
        /CLOCKS_PER_SEC;
    printf("\n Total Time=%lf\n",Total_Time);
}
Encrypt(mpz_t p,mpz_t g)
{
    mpz_t Randomization_Param,p_minus_2,. Public_Key;
    mpz_init_set_ui(Randomization_Param,0);
    mpz_init(p_minus_2);
    mpz_init(Public_Key);
    mpz_sub_ui(p_minus_2,p,2);
    mpz_t C1,C2;
    mpz_init_set_ui(C1,0);
    mpz_init_set_ui(C2,0);
    mpz_t M;
    mpz_init(M);
    FILE *fileptr;char File_Name[20];
    // printf("\n Enter File Name Where Embedded
    Message is Stored\n");
    // scanf("%s",&File_Name);
    // fileptr=fopen(File_Name,"r");
    fileptr=fopen("Message","r");
    gmp_fscanf(fileptr,"%Zd",&M);
    fclose(fileptr);
    // gmp_printf("\n M=%Zd \n",M);
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state,random());
    // gmp_printf("\np_minus_2 = %Zd \n",p_minus_2);
    while(mpz_cmp_ui(Randomization_Param,0)==0)
        mpz_urandomm(Randomization_Param,
                state,p_minus_2);
    // gmp_printf("\nRandomization_Parm =
    %Zd\n",Randomization_Param);
    mpz_powm(C1,g,Randomization_Param,p);
    // printf("\n Enter File Name Where Public Key is Stored\n");
    // scanf("%s",&File_Name);
    // fileptr=fopen(File_Name,"r");
    fileptr=fopen("richa","r");
    gmp_fscanf(fileptr,"%Zd",&Public_Key);
    fclose(fileptr);
    mpz_powm(C2,Public_Key,
            Randomization_Param,p);
    mpz_mul(C2,C2,M);
    mpz_mod(C2,C2,p);
    fileptr=fopen("encryption","w");
    gmp_fprintf(fileptr,"%Zd ",C1);
    gmp_fprintf(fileptr,"%Zd ",C2);
    fclose(fileptr);
}
Decrypt(mpz_t p,mpz_t g)
{
    mpz_t Secret_Key;
    mpz_init(Secret_Key);
    FILE *fileptr;char File_Name[20];
    // printf("\n Enter File Name Where Secret Key is Stored\n");
    // scanf("%s",&File_Name);
    // fileptr=fopen(File_Name,"r");
    fileptr=fopen("richa_sec","r");
    gmp_fscanf(fileptr,"%Zd",&Secret_Key);
    fclose(fileptr);
    // gmp_scanf("%Zd",&Secret_Key);
    mpz_t M,C1,C2,I;
    mpz_init_set_ui(M,0);
    mpz_init_set_ui(C1,0);
    mpz_init_set_ui(C2,0);
    mpz_init_set_ui(I,0);
    mpz_sub_ui(I,p,1);
    mpz_sub(I,I,Secret_Key);
    // printf("\n Enter File Name Where Encrypted Message is Stored\n");
    // scanf("%s",&File_Name);
    // fileptr=fopen(File_Name,"r");
    fileptr=fopen("encryption","r");
    gmp_fscanf(fileptr,"%Zd",&C1);
    gmp_fscanf(fileptr,"%Zd",&C2);
    fclose(fileptr);
    mpz_powm(M,C1,I,p);
    mpz_mul(M,M,C2);
    mpz_mod(M,M,p);
    // gmp_printf("\n M=%Zd \n",M);
}
