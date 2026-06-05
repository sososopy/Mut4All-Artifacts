
template<typename T>
struct Foo
{
  Foo() noexcept = default;
  ~Foo() noexcept(true) = default;
};

int main(int argc, char** argv)
{
  Foo<int> f;
  return 0;
}

