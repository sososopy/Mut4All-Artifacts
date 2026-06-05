

struct Outer {
    void f() { }
    class C { };
    
    class Inner {
        friend class C;
        friend void f();
        static const int i = 0;
    };
};

void f() { int i = Outer::Inner::i; }
class C { int i = Outer::Inner::i; };

int main() { }

