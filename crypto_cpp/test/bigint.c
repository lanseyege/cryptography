#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//int n = 1000, base = 10;

#define MAX_LEN 1000
#define base 10

int A[MAX_LEN], B[MAX_LEN], C[MAX_LEN];

void set_zero() {
    for (int i = 0; i < MAX_LEN; i++) {
        A[i] = 0;
        B[i] = 0;
        C[i] = 0;
    }
}

void print_res(int res[], int res_len) {
    printf("\n");
    for (int i = 0; i < res_len; i++) {
        printf("%d", res[ MAX_LEN - res_len + i]);
    }
    printf("\n");
}

void add(char *left, char *right) {
    set_zero();
    int left_len = strlen(left);
    int right_len = strlen(right);
    printf("left array length: %d, right array length: %d\n", left_len, right_len);
    if (left_len >= MAX_LEN || right_len >= MAX_LEN) {
        printf("length oversize\n");
        return ;
    }
    int temp = 0, ord = 0, i, res_len;
    for(i = 0; i < left_len && i < right_len; i++) {
        //C[MAX_LEN - i - 1] += ord ;
        temp = left[left_len - i - 1] - '0' + right[right_len - i - 1] - '0' + ord;
        ord = (int)(temp / base);
        C[MAX_LEN - i - 1] = temp % base;
        printf("i is %d, left is: %d, right is: %d, res is %d\n", i, (int)(left[left_len - i - 1]),(int)(right[right_len - i - 1]),C[MAX_LEN - i - 1]);
    }
    printf("i is %d\n", i);
    if ( i < left_len && i == right_len) {
        int j = i;
        for (; j < left_len; j ++) {
            //C[MAX_LEN - j - 1] += ord;
            temp = left[left_len - j - 1] - '0' + ord; //+ int(right[right_len - j - 1]) ;
            ord = (int)(temp / base);
            C[MAX_LEN - j - 1] = temp % base;
            printf("j is %d, left is %d, res is %d:\n", j, left[left_len - j - 1], C[MAX_LEN - j - 1] );
        }
        if (ord == 1) {
            C[MAX_LEN - j - 1] = ord;
            res_len = left_len + 1;
        }else{
            res_len = left_len ;
        }
    }
    else if (i < right_len && i == left_len) {
        int j = i;
        for (; j < right_len; j ++) {
            //C[MAX_LEN - j - 1] += ord;
            temp = right[right_len - j - 1] - '0' + ord; //+ int(right[right_len - j - 1]) ;
            ord = (int)(temp / base);
            C[MAX_LEN - j - 1] = temp % base;
            //printf("j %d is %d:\n", j, C[MAX_LEN - j - 1] );
            printf("j is %d, left is %d, res is %d:\n", j, right[right_len - j - 1], C[MAX_LEN - j - 1] );
        }
        if (ord == 1) {
            C[MAX_LEN - j - 1] = ord;
            res_len = right_len + 1;
        }else{
            res_len = right_len ;
        }
    }
    else if ( i == left_len && i == right_len) {
        if (ord == 1) {
            C[MAX_LEN - i - 1] = ord;
            res_len = right_len + 1;
        }else{
            res_len = right_len ;
        }
    }
    printf("res len %d\n", res_len);
    print_res(C, res_len);
    //char res[res_len];
    //for (i = 0; i < res_len; i++) {
    //    res[res_len - i - 1] = char(C[n - i - 1]);
    //}
    return ; 
}

void multiply(char left[], char right[]) {
    set_zero();
       
}

