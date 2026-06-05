
#include <iostream>
using namespace std;

struct Parent{
    int data = 0;
};

template <typename Context>
class Child : public Parent{
public:
    static int func(){
        Child tp;
        int x = tp.data;
        return x;
    }
};

template class Child<int>;


int main(){
    
}

