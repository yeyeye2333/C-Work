#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int radixsort(char**a,int k,int al,int n);
int* smallestTrimmedNumbers(char** nums, int numsSize, int** queries, int queriesSize) {
    int *back=(int *)malloc(sizeof(int)*queriesSize);
    for(int c=0;c<queriesSize;c++)
    {
        back[c]=radixsort(nums,queries[c][0],numsSize,queries[c][1]);
    }
    return back;
}
int main()
{
    char tem2[4][4]={"102","473","251","814"};
    char*tem[4]={tem2[0],tem2[1],tem2[2],tem2[3]};
    char**nums=tem;
    int tem3[4][2]={1,1,2,3,4,2,1,2};
    int *tem4[4]={tem3[0],tem3[1],tem3[2],tem3[3]};
    int **queries=tem4;
    int *back=smallestTrimmedNumbers(nums,4,queries,4);
    printf("%d,%d",back[0],back[1]);
    free(back);
    return 0;
}
int radixsort(char**a,int k,int al,int n)
{
    int maxdi=0;
    while(a[0][maxdi]!='\0')
    {
        maxdi++;
    }
    char result[al][maxdi+1];
    char cpy[al][maxdi+1];//防止原数组被修改
    for(int c=0;c<al;c++)
    memcpy(cpy[c],a[c],(maxdi+1)*sizeof(char));
    int tem[n][al];
    for(int c=1;c<=n;c++)
    {
        int bucket[10]={0};
        for(int cc=0;cc<al;cc++)
            bucket[cpy[cc][maxdi-c]%'0']++;
        for(int cc=1;cc<10;cc++)
            bucket[cc]=bucket[cc]+bucket[cc-1];
        for(int cc=al-1;cc>=0;cc--)
        {
            memcpy(result[bucket[cpy[cc][maxdi-c]%'0']-1],cpy[cc],(maxdi+1)*sizeof(char));
            tem[c-1][bucket[cpy[cc][maxdi-c]%'0']-1]=cc;
            bucket[cpy[cc][maxdi-c]%'0']--;
        }
        for(int cc=0;cc<al;cc++)
        {
            memcpy(cpy[cc],result[cc],maxdi*sizeof(char));
        }
    }
    int tmp2=k-1;
    int tmp3;
    for(int c=n-1;c>=0;c--)
    {
        tmp3=tem[c][tmp2];
        tmp2=tmp3;
    }
    return tmp2;
}