

class A {
private:
    void method(int i) {
        (void) i;
    }

public:
    void publicMethod() {
        auto lambda = [&] (const auto k) {
            method(k);
        };
        lambda(42);
    }
};

int main()
{
    A a;
    a.publicMethod();
    return 0;
}
