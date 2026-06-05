
struct Foo { unsigned x_; };
  
int main(int argc, char *argv[])
{
  const Foo a{3};
  float f[2][a.x_]{};
  return 0;
}
