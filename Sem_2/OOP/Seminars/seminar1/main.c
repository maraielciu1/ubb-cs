#include <stdio.h>
/*
 * read a sequence of numbers till 0 is encountered, print the sum of the numbers and the longest continuous
 * sequence where all the elements are equal
 */

void increase(int x) // a copy of x with a different address -> it does not modify the x from the main
{
    x=x+1;
}

void increase1(int *x) // modifies the x from the main by deref.
{
    *x=*x+1;
}

typedef struct
{
    int size;
    int val[100];
}Vector;

Vector read()
{
    int x=1;
    Vector v;
    v.size=0;
    while(x) {
        printf("The value: ");
        scanf("%d", &x);
        if (x == 0){
            return v;}
        else {
            v.val[v.size] = x;
            v.size++;

        }
    }
    return v;
}

int sum(Vector v)
{
    int s=0;
    for(int i=0;i<v.size;i++)
    {
        s+=v.val[i];
    }
    return s;
}

Vector subseq(Vector v)
{
    Vector s;
    s.size=0;
    int x=v.val[0];
    int max=1,cmax=0,maxi=0,maxj=0,ci=0,cj=0;
    for(int i=1;i<v.size;i++)
    {
        if(x==v.val[i])
        {
            cmax++;
            cj++;
        }
        else
        {
            if(cmax>max)
            {
                max=cmax;
                maxi=ci;
                maxj=cj;
            }
            cmax=1;
            ci=cj=i;
        }
        x=v.val[i];
    }
    if(cmax>max)
    {
        maxi=ci;
        maxj=cj;
    }
    for(int i=maxi;i<=maxj;i++)
    {
        s.val[s.size]=v.val[i];
        s.size++;
    }
    return s;
}

void print_menu()
{
    printf("Choose an option: \n");
    printf("0. Exit\n");
    printf("1. Read the vector\n");
    printf("2. Sum of the elements\n");
    printf("3. LCS of the vector\n");

}

//int main() {

//    int x=0;
//
//    printf("Input x: ");
//    scanf("%d",&x);
//    increase1(&x);
//    printf("Value of x=%d \n",x);
//
//    char name[20];
//    printf("Input a name: ");
//    scanf("%s",name);
//
//    printf("%p\n",name);
//    printf("%p\n", &name[0]);
//    printf("%p\n", &name[1]);
//    printf("%p\n", &name[2]);
//    printf("%p\n", &name[3]);
//
//    printf("First character: %c\n", name[0]);
//    printf("First character is: %c\n", *name);
//
//    printf("First character: %c\n", name[1]);
//    printf("First character is: %c\n", *(name+1));


//    Vector v=read();
//    printf("%d\n",v.size);
//    printf("%d",sum(v));
//    Vector s= subseq(v);
//    for(int i=0;i<s.size;i++)
//    {
//        printf("%d ",s.val[i]);
//    }
void also_main()
{    Vector v;
    while(1)
    {
        print_menu();
        int input, s=0;
        printf(">");
        scanf("%d",&input);
        if (input==0)
        {
            break;
        }
        switch (input)
        {
            case 1: v=read(); break;
            case 2:
            {
                s=sum(v);
                printf("%d\n",s);
                break;
            }
            case 3:
            {
                Vector s= subseq(v);
                for(int i=0;i<s.size;i++)
                {
                    printf("%d ",s.val[i]);
                }
                break;
            }
        }
    }}
    //return 0;
//}
