
struct S {
  char c;
  struct T {} T::*b[];
} a;

void f ()
{
  a.c = 1;
}

