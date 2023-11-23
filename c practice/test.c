#include<stdio.h>
int main()
{
    FILE*ff;
    if((ff=fopen("第十七章/testfile","r+"))==NULL)
    {
        printf("fuck");
    }
    char a[40];
    rewind(ff);
    if(fscanf(ff,"%s",a)==1)
    {
        printf("wow");
    }
    fclose(ff);
    return 0;
}