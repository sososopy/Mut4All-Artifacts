
class foo
{
  typedef int type;
};

template <typename T>
foo::type bar()
{
  return 1;
}

int main()
{
  return bar<char>();
}
