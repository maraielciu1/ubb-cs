#include <stdio.h>

int mini(int a, int b);

int main() {
    char buffer[101];
    char filename[] = "min.txt";
    char access_mode[] = "w";
    int len;
    int result;

    // how many numbers are read
    printf("how many numbers: ");
    scanf("%d", &len);

    printf("Numbers: ");

    if (len > 0) {
        // the first number in the list is considered the minimum
        int min;
        scanf("%d", &min);
        result = min;
        len--;

        while (len > 0) {
            int number;
            scanf("%d", &number);
            result = mini(result, number);
            len--;
        }

        
        printf("%x", result);
           
    }

    return 0;
}