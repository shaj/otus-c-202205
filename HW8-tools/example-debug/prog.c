#include <stdio.h>


int divide(int a, int b)
{
    return a / b;
}

int main(void)
{
    int x = 5, y = 2;
    printf("%d ", divide(x, y));

    x = 3; y = 0;

    printf("%d ", divide(x, y));

    return 0;
}
