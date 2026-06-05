
class base
{
protected:
  using type = int;
};

template <typename T>
class derive: public base
{
  using typename base::type;
public:
  void foo(type a)
  {
    auto f = [a](type x){ return x == a; };
    f(32);
  }
};

int main()
{
  derive<char> d;
  d.foo(23);
}
