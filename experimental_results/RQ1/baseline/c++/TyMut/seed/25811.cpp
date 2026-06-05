
class A {
};

class B : public A {
};

int main() {
    A* a;
    B* b = new B();
    a = dynamic_cast<const A*>(b);
    return 0;
}
