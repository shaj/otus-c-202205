#include <stdio.h>


int main(void)
{
    int number;
    printf("Enter the number: ");
    scanf ("%d", &number );

    int j;
    for(int i = number; i > 0; i--)
    {
        j *= i;
    }
    printf("The factorial of %d is %d\n", number, j);

    return 0;
}
