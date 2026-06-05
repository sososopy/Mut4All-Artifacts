extern "C" int puts(const char*);

struct X
{
  X() { }

  X(const X&) { puts("copy"); }
  X(X&&) { puts("move"); }

  X& operator=(const X&) { puts("copy assign"); return *this; }
  X& operator=(X&&) { puts("move assign"); return *this; }
};

struct wrap
{
  wrap(const X& x) : wrapped(&x) { }

  const X* wrapped;

  operator const X&() const { return *wrapped; }
};

int main()
{
  X x1, x2;
  wrap w(x2);
  x1 = w;
}

