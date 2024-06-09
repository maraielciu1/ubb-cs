//
// Created by Mara Ielciu on 02.03.2024. problem 4
//
#include <stdio.h>
#include <stdlib.h>

void print_menu()
{
    printf("Choose your option:\n");
    printf("a. Root of a number with a given precision\n");
    printf("b. Longest subsequence with the difference of 2 adjacent numbers a prime number\n");
    printf("c. Exit\n");
}

typedef struct {
    int size;
    int val[100];
}Vector;

double root(double number, double precision)
{
    double middle, dr=number,st=0;
    while(dr-st>precision)
    {
        middle = (dr+st)/2.0;
        if(middle*middle>number)
        {
            dr = middle;
        }
        else
            st = middle;
    }
    return (st+dr)/2.0;

}

int is_prime(int number)
{
    int cnt=0;
    for(int i=2;i<number;i++)
    {
        if(number%i==0)
            cnt++;
    }
    if(cnt!=0)
        return 0;
    else
        return 1;
}

Vector subseq(Vector v)
{
    Vector s;
    s.size=0;
    int value, max = 0, cmax = 0, starti = 0, endi = 0, startm = 0, endm = 0;
    value = v.val[0];
    for (int i = 1; i < v.size; i++) {
        int diff = abs(v.val[i] - value);
        if (is_prime(diff)) {
            cmax++;
            endi = i;
        } else {
            if (cmax > max) {
                max = cmax;
                endm = endi;
                startm = starti;
            }
            cmax = 0;
            starti = i;
            endi = i;
        }
        value = v.val[i];

    }
    if (startm == endm)
        s.size = 0;
    else {
        for (int i = startm; i <= endm; i++)
            s.val[s.size++]=v.val[i];
    }
    return s;
}

int main(){

    printf("Hello!");
    while(1)
    {
        print_menu();
        char option='c';
        printf(" > ");
        scanf(" %c",&option);
        if(option == 'a')
        {
            double number, precision,result=0;
            printf("Give the value of the number: ");
            scanf("%lf",&number);
            printf("Give the precision: ");
            scanf("%lf",&precision);
            result=root(number,precision);
            printf("The result is %lf \n", result);
        }
        else
            if (option=='b')
            {
                Vector v,s;
                v.size=0;
                printf("Give number of elements: ");
                scanf("%d",&v.size);
                printf("Give the elements: ");
                for(int i=0;i<v.size;i++)
                {
                    scanf("%d",&v.val[i]);
                }
                s.size=0;
                s= subseq(v);
                if(s.size==0)
                    printf("There is no subsequence with that property\n");
                else
                {
                    printf("The subsequence is: ");
                    for (int i=0;i<s.size;i++)
                        printf("%d ",s.val[i]);
                    printf("\n");
                }
            }
            else
                if(option == 'c')
                    break;
                else
                    printf("Invalid option\n");

    }

    return 0;
}