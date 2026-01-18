#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* --- MINI-LANG RUNTIME START --- */
double calc_mean(double* arr, int n) {
    double sum = 0; for(int i=0; i<n; i++) sum+=arr[i]; return sum/n;
}
double calc_stdev(double* arr, int n) {
    double m = calc_mean(arr, n); double sum = 0;
    for(int i=0; i<n; i++) sum += pow(arr[i]-m, 2);
    return sqrt(sum/n);
}
double sample_normal(double param) {
    // Box-Muller transform
    double u = (double)rand() / RAND_MAX; double v = (double)rand() / RAND_MAX;
    return sqrt(-2.0 * log(u)) * cos(2.0 * 3.14159 * v) * param;
}
/* --- MINI-LANG RUNTIME END --- */

int main() {
    srand(time(NULL)); // Seed RNG
int x = 5;
int y = 10;
x = x + y;
    return 0;
}
