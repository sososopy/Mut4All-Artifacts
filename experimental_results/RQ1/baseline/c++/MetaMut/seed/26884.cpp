
#include <iostream>
void fun(double _v)
{
    std::cout<<"_v="<<_v<<std::endl;
    long long int var=static_cast< long long int >(_v*1000.);
    std::cout<<"var="<<var<<std::endl;
}
int main(int ac, char** av)
{
    fun(33.33);
    return 0;
}
