
#include<iostream>
using namespace std;

struct A{
    unsigned int const v;
    A():v(10){}
    void test_A();
};

struct B{
    A const*const&a;
    B(A const*const&aa):a(aa){}
    void test_B(){
        unsigned int it=0;
        cout<<"Value of constant a->v: "<<a->v<<endl;
        do{
            cout<<it<<',';
        }while(++it<a->v);
        cout<<endl<<"Value of constant a->v: "<<a->v<<endl;
        //cout<<endl<<"Value of constant a->v:"<<' '<<a->v<<endl;


    }
};

void A::test_A(){
    B*b=new B(this);
    b->test_B();
    delete b;
}

int main(){
    A*a=new A();
    a->test_A();
    delete a;
    return 0;
};
