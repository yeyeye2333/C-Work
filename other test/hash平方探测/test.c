#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"hash1.h"
static int nextprime(int val);
static int hashjudge(char*key,int tablesize);
static int findbase(char*key,hashtab*h);
void init(hashtab *h,int table,int words)
{
    h->table=(hash*)malloc(sizeof(hash));
    h->table->tablesize=nextprime(table);
    h->table->cells=(struct ele*)malloc(sizeof(struct ele)*(h->table->tablesize));
    for(int c=0;c<h->table->tablesize;c++)
    {
        h->table->cells[c].words=(char*)malloc(sizeof(char)*(words+1));
        h->table->cells[c].is=emp;
    }
}
static int hashjudge(char*key,int tablesize)
{
    int size=strlen(key);
    int ret=0;
    for(int c=0;c<size;c++)
    {
        ret+=(ret*27)+key[c];
    }
    return ret%tablesize;
}
static int findbase(char*key,hashtab*h)
{
    int i=1;
    int count=0;
    int index;
    index=hashjudge(key,h->table->tablesize);
    while(h->table->cells[index].is!=emp&&
        strcmp(h->table->cells[index].words,key)!=0)
    {
        index+=2*++count-1;
        if(index>h->table->tablesize)
        {
            index%=h->table->tablesize+1;
        }
        if(i<=h->table->tablesize/2+1)i++;
        else return -1;
    }
    return index;
}
int find(char*key,hashtab*h)
{
    int index;
    if((index=findbase(key,h))==-1)return -1;
    else if(strcmp(h->table->cells[index].words,key)==0)
    return index;
    return -1;
}
int insert(char*key,hashtab*h)
{
    int index;
    index=findbase(key,h);
    if(index==-1)return 0;
    if(strcmp(h->table->cells[index].words,key)!=0)
    {
        h->table->cells[index].is=ocp;
        memcpy(h->table->cells[index].words,key,strlen(key)+1);
        return 1;
    }
    return 0;
}
static int nextprime(int val)
{
    int i,k;
    k=(int)sqrt( (double)val );
    while(i<=k)
    {
        for(i=2;i<=k;i++)
            if(val%i==0)
                break;
        val++;
    }
    return val;
}

int main()
{
    hashtab h;
    init(&h,10,10);
}