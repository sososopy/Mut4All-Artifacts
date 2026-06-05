
#include <iostream>

using namespace std;

struct Vector {
  double x;
  double y;
  double z;
};

class Baz {
  private:
    Vector r;
    Vector u;

  public:
    Baz(const Vector& p, const Vector& v) : r{p.x,p.y,p.z}, u{v.x,v.y,v.z} {}

    const Vector& get_u() const {
      return(u);
    };

    void write() const {
      cout << "r: " << r.x << "," << r.y << "," << r.z << "; u: " << u.x << "," << u.y << "," << u.z << endl;
    };
};

class Foo {
  private:
    const Baz& b;

  public:
    Foo(const Baz& b_) : b{b_} {}
    void bar();
};

void Foo::bar() {
  b.write();
  int iters{10};
  b.write();
}

int main(void) {
  Baz b{Vector{0.5, 0.5, 0.5}, Vector{1e6, 0.1, 0.1}};
  Foo f{b};
  f.bar();
}
