
#include <iostream>
#include <fstream>
using namespace std;


struct A
{
    int z=0;

    template<typename T>
    void func(T &y)
    {
        //cout<<typeid(T).name()<<endl;
        y<<z<<endl;
    }
};


template<typename T,typename T1>
T& operator<<(T &y,T1 &y1)
{
    y1.func(y);
    return y;
}

int main()
{
    A a;
    a.z=10;
    fstream fs("a.txt",ios::out);

    fs<<a; //this doesnt compile ????  ( I do have a walk around, but curious)
    cout<<a; //this works
    return 0;
}
