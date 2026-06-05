
struct B { int i; };
struct D : B {
    int x;
    int y;
};

int
main ()
{
  D d = {.x=1, .y=2};
  (void)d;
}

