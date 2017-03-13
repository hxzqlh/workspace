#include <stdio.h>
#include "caculate.h"

int add(int a,int b)
{
    int foo = sub (a,b);
    int bar = mul (a,b);

    printf ("sub: %d mul:%d\n", foo, bar);
    return (a + b);
}

int sub(int a, int b)
{
    return (a - b);
}

int mul(int a, int b)
{
    return (a * b);
}

int div(int a, int b)
{
    return (a / b);
}
