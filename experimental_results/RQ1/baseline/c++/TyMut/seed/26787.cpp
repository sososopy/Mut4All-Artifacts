
#include <stdio.h>
#include <cmath>
#include <math.h>


int main( void )
{
    double sq3 = 9.1;
    double ret3 = sqrtf(sq3);
    printf("%f\n", ret3);

    float sq4 = 9.1;
    double ret4 = sqrtf(sq4);
    printf("%f\n", ret4);

    double sq1 = 9.1;
    double ret1 = sqrt(sq1);
    printf("%f\n", ret1);

    float sq2 = 9.1;
    double ret2 = sqrt(sq2);
    printf("%f\n", ret2);

    return 0;
}
