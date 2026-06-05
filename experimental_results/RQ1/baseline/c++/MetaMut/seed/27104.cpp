

#include <iostream>
using namespace std;
int func(int a, int b)
{
    cout << a << " " << b <<"\n";
    return a << b;
}
int main()
{
    int i = 2;
    cout << func(++i, i++) << "\n";
};

