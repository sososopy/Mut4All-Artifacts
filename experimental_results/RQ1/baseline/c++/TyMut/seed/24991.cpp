

struct X {
  int f();
};

void g(int);

int main()
{
  X x;
  g(
      x.f
   );
}

