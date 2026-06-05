
#include <cstdio>
#include <cstdlib>

class A  {
public:
    __attribute__((noreturn)) virtual bool foo() {
        exit(-1);
    }
};

class B : public A {
public:
    bool foo() {
        printf("foo\n");
    }
};

int main(int argc, char** argv) {

    volatile int a = 3;
    A* b = new B;
    switch(a) {
        case 3:
            b->foo();
            printf("3\n");
            break;
        case 4:
            printf("4\n");
            break;
    }
}

 