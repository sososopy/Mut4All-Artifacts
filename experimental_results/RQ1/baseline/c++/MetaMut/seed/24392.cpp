
struct X {
  virtual void f() = 0;
  void g() { X::f(); }
};

struct Y : X {
  void f() { }
};

int main()
{
  Y y;
  y.g();
}

void X::f() { }
