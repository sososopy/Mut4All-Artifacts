#include<typeinfo>

struct A{
    virtual void f();
};
struct B{
    void f();
};

bool compare(A& a, A& b){
    return typeid(a)==typeid(b);
}