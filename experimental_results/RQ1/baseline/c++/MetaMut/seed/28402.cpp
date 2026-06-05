
template <typename T> void g(T, T) { }

class C {
  static int XYZZY;
  int x;
  void f() { g(x, XYZZY); }
};

int main()
{
    return 0;
}

