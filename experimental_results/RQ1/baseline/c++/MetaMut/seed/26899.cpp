
#include <iostream>

int c, d;

struct a
{
  a (int i) { if (i) throw i; c++; }
  ~a () { d++; }
};

void check (void (*f) ())
{
  try
  {
    c = d = 0;
    f ();
  }
  catch (int)
  {
    std::cerr << (c != 1 ? "constructor not completed exactly once\n" :
                  d == 0 ? "destructor not called\n" :
                  d == 1 ? "destructor called\n" :
                           "destructor called more than once\n");
    return;
  }
  std::cerr << "exception not thrown\n";
}

int main ()
{
  struct s { a x, y; };
  check ([] { s t { 0, 1 }; });
  check ([] { s { 0, 1 }; });
  check ([] { a t[2] { 0, 1 }; });
  check ([] { (a[2]) { 0, 1 }; });
  check ([] { std::initializer_list <a> t { 0, 1 }; });
  check ([] { std::initializer_list <a> { 0, 1 }; });
}
