
#include <iostream>
 
enum en
{
        a,b,c
};
 
struct B
{
template<en N>
struct A
{
        const int X = N; // #1
};
 
};
 
int main(int argc, const char *argv[])
{
        B::A<en::b> n;
        std::cout << "n.X = " << n.X << "\n";
        
}

