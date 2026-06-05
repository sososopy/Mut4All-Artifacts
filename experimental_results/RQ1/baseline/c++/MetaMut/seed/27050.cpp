
#include <iostream>

using namespace std;

auto GetSum(auto a, auto b)
{
    return (a + b);
}


int main()
{
    auto temp = GetSum(10, 20.5);
    cout << "The sum is " << temp << endl;
    
    return 0;
}

