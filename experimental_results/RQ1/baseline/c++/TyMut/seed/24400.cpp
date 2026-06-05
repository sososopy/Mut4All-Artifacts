
void f( int );
namespace N {
    struct A
    {
       friend void f(int);
       friend void f(double);
    };
    void g() {
        f(0);
    }
}

