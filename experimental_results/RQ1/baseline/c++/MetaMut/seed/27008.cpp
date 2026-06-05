

class A {
public:
    A( int ) {  }
};

class B: virtual public A {
public:
    virtual void do_something() = 0;
};

class C: public B {
public:
    C(): A( 1 ) {  }
    virtual void do_something() {  }
};

int main() {
    C c;
    return 0;
}
