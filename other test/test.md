# 力扣2343. 裁剪数字后查询第 K 小的数字
>[基数排序（英语：Radix sort）是一种非比较型整数排序算法，其原理是将整数按位数切割成不同的数字，然后按每个位数分别比较。由于整数也可以表达字符串（比如名字或日期）和特定格式的浮点数，所以基数排序也不是只能使用于整数。](https://zh.wikipedia.org/zh-hans/%E5%9F%BA%E6%95%B0%E6%8E%92%E5%BA%8F)
- 基数排序实际为多个桶排序的结合，对整数的基数排序及其驱动程序如下
```c
#include<stdio.h>
void radixsort_int(int *a,int n);
int main()
{
    int arr[6]={444,152,415,151,223,11};
    for (int i = 0; i < 6; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    radixsort_int(arr,6);
    for (int i = 0; i < 6; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
void radixsort_int(int *a,int n)
{
    int digit=1;
    int max=a[0];
    for(int c=1;c<n;c++)
    {
        max=max>=a[c]?max:a[c];
    }
    int maxdi=1;
    while((max/=10)>0)
    {
        maxdi++;
    }
    int cc;
    int result[n];
    for(int c=0;c<maxdi;c++)//依最高位决定循环次数
    {
        int buckets[10]={0};
        for(cc=0;cc<n;cc++)
            buckets[(a[cc]/digit)%10]++;//纪录每个位的个数
        for(cc=1;cc<10;cc++)
            buckets[cc]=buckets[cc-1]+buckets[cc];//记录对应位应在的数组下标+1
        for(cc=n-1;cc>=0;cc--)
        {
            int ccc=(a[cc]/digit)%10;
            result[buckets[ccc]-1]=a[cc];//依次赋值
            buckets[ccc]--;
        }
        for(cc=0;cc<n;cc++)
            a[cc]=result[cc];
        digit*=10;
    }
}
```
- 应用
***Q***
给你一个下标从 0 开始的字符串数组 nums ，其中每个字符串 长度相等 且只包含数字。

再给你一个下标从 0 开始的二维整数数组 queries ，其中 queries[i] = [ki, trimi] 。对于每个 queries[i] ，你需要：

    将 nums 中每个数字 裁剪 到剩下 最右边 trimi 个数位。
    在裁剪过后的数字中，找到 nums 中第 ki 小数字对应的 下标 。如果两个裁剪后数字一样大，那么下标 更小 的数字视为更小的数字。
    将 nums 中每个数字恢复到原本字符串。

请你返回一个长度与 queries 相等的数组 answer，其中 answer[i]是第 i 次查询的结果。

提示：

    裁剪到剩下最右边 x 个数位的意思是不断删除最左边的数位，直到剩下 x 个数位。
    nums 中的字符串可能会有前导 0 。
***A***
可将字符串转化类比为整数
```c
int* smallestTrimmedNumbers(char** nums, int numsSize, int** queries, int queriesSize, int* queriesColSize, int* returnSize) {
    int *back=(int *)malloc(sizeof(int)*queriesSize);
    for(int c=0;c<queriesSize;c++)
    {
        back[c]=radixsort(nums,queries[c][0],numsSize,queries[c][1]);
    }
    *returnSize=queriesSize;
    return back;
}
int radixsort(char**a,int k,int al,int n)
{
    int maxdi=0;
    while(a[0][maxdi]!='\0')
    {
        maxdi++;
    }
    char result[al][maxdi+1];
    int tem[n][al];
    for(int c=1;c<=n;c++)
    {
        int bucket[10]={0};
        for(int cc=0;cc<al;cc++)
            bucket[a[cc][maxdi-c]%'0']++;
        for(int cc=1;cc<10;cc++)
            bucket[cc]=bucket[cc]+bucket[cc-1];
        for(int cc=al-1;cc>=0;cc--)
        {
            for(int ccc=0;ccc<=maxdi;ccc++)
            {result[bucket[a[cc][maxdi-c]%'0']-1][ccc]=a[cc][ccc];}

            tem[c-1][bucket[a[cc][maxdi-c]%'0']-1]=cc;
        }
        for(int cc=0;cc<al;cc++)
        {
            for(int ccc=0;ccc<maxdi;ccc++)
            {
                a[cc][ccc]=result[cc][ccc];
            }
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
```