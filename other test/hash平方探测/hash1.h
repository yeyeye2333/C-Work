enum status{emp,del,ocp};
struct ele
{
    char *words;
    enum status is;
};
typedef struct{
    struct ele *cells;
    int tablesize;
}hash;
typedef struct
{
    hash*table;
}hashtab;
void init(hashtab *h,int table,int words);
int find(char*key,hashtab*h);
int insert(char*key,hashtab*h);
