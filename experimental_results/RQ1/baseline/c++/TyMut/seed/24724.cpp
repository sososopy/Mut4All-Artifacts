
#include <stdio.h>
struct Base{
    virtual void f() {printf("base\n");}
};
struct Child : Base{
    void f()override {printf("child\n");}
};
alignas(Child)
char buf[sizeof(Child)];
union Ptr{
    decltype(::buf) buf;
    Base base;
};
int main(){
    new(buf) Child;
    Ptr *ptr = reinterpret_cast<Ptr*>(buf);
    //-------------------printf: base---//
    ptr->base.f();                      //
    Base &b = ptr->base;                //
    b.f();                              // clang print "child".
    //----------------------------------//
    //-------------------printf: child--//
    Base *b2 = &ptr->base;              //
    b2->f();                            //
    //----------------------------------//
}
