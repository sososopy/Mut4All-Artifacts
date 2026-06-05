struct Trace
{
  ~Trace() {}
};

struct Foo {
    Trace t;
};

struct Bar {
    Bar(Foo f = {}) {}
};

int main() {
    Bar b;
}

