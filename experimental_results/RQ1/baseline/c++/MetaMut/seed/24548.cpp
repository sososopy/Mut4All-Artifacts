extern "C" int printf(const char*, ...);

const char* locn = nullptr;

struct X
{
  X() {
    printf("%p cons in %s\n", this, locn);
  }
  X(X const& x) {
    printf("%p copy %p in %s\n", this, &x, locn);
  }
  ~X() {
    printf("%p dest\n", this);
  }
};

int main()
{
  locn = "main";
  auto f = [] (X xx)
  {
    locn = "lambda";
    printf("%p is &xx in lambda\n", &xx);
    return xx;
  };
  X (*ff) (X) = f;
  ff ( X{} );
}


