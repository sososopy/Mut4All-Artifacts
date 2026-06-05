
struct a
{
  explicit a () { }
};

void foo (a)
{
}

int main ()
{
  foo ({ });
}
