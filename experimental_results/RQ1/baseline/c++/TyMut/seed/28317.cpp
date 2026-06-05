
template<typename>
constexpr bool b = true;

template<typename T>
concept C = b<T>;

template<typename T, typename U>
class foo
{
public:
  void bar() const requires C<T> {}

  void bar() const requires (C<T> && C<U>) {}
};

int main()
{
  foo<int, int> x;
  
  x.bar();

  return 0;
}
