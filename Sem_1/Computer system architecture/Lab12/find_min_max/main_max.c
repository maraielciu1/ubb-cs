#include <stdio.h>

#pragma comment(lib, "legacy_stdio_definitions.lib")

int findmax(int nr1, int nr2);

int main() {
    int n,nr,max=0;
    printf("Give how many numbers to be read: ");
    scanf("%d",&n);
    scanf("%d",&max);
    for(int i=2;i<=n;i++)
    {    scanf("%d",&nr);
    max=findmax(max,nr);}
    printf("The maximum value is: %x",max);
    return 0;
}