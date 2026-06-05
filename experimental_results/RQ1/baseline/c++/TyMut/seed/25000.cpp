
#include <cstdint>
int main(void)
intmax_t i2=2,i3=3;
long double ld4=5,ld5=5;
long double 
dresult1=i2%ld4,dresult2=ld4%i2,dresult3=ld4%ld5,dresult4=i2%i3;
intmax_t 
iresult1=i2%ld4,iresult2=ld4%i2,iresult3=ld4%ld5,iresult4=i2%i3;
//if this works, it's because the optimizer blew the code away
return 0;


