
class Foo { };

class Parent {
public:
    virtual void func();
};

long foo();

template <class Foo>
class Child: public Parent {
public:
    void func() {
        register long A asm("rax");
        register long B asm("rbx");
        A = foo();
        B = foo();
        __asm volatile("and %1, %0" : "+r" (A) : "r" (B));
    }

};

void *check_source = new Child<Foo>();

g++ -O1, g++ -O3: Neither register is used as assigned.
         Second call to foo() as if return is in %rbx!
        call    _Z3foov
        movq    %rax, %rbx
        call    _Z3foov
        movq    %rbx, %rdx
        and %rax, %rdx

