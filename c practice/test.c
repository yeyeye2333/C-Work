#include<stdio.h>
int main()
{
    char ch;
    printf("print a capital:");
    scanf("%c",&ch);
    for(int c=0;c<(ch-'A'+1);c++)
    {
        for(int cc=0;cc<(ch-'A'+1-c);cc++)
        {
            printf(" ");
        }
        for(int cc=0;cc<=c;cc++)
        {
            printf("%c",cc+'A');
        }
        for(int cc=c;cc>0;cc--)
        {
            printf("%c",'A'+cc-1);
        }
        printf("\n");
    }
    return 0;
}