

class O {};

class D : public virtual O {
public:
    virtual float foo() const = 0;
};

class B : public D {
public:
    B(float, float) {}
};

class R : public B {
public:
    R() : B{1.f, 1.f} {}
    virtual float foo() const override { return 0.f; }
};

int main() {
    R r;
}

