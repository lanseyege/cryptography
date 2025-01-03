#include <stdio.h>
#include "bigint.c"


int main() {
    
    char left[] = "12344556677888901881821";
    char right[] = "12344556677888901881821997";
    printf("left %s, right %s\n", left, right);
    add(left, right);

}

