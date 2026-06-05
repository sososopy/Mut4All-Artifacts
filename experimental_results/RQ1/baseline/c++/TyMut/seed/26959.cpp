
template<class T>
struct Foo
{
  using type = T;
};

template<class T>
class Bar : public Foo<T>
{
  using Base = Foo<T>; // the combination of this and the public export of a type from Base below, causes this example to compile even though it shouldn't
  // typedef Foo<T> Base; // when using typedef instead of using, it correctly fails to compile
public:
  // without this public export, Base is correctly recognized as private, maybe it erroneously make Base itself public?

  // whether written as 'typedef' or 'using' doesn't matter
  using type = typename Base::type;
};

Bar<int>::Base x; // shouldn't compile, Base is private

int main()
{
  return 0;
}
