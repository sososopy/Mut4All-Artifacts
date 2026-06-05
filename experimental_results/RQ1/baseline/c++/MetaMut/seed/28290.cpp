
struct Impl
{
  constexpr Impl(std::initializer_list<int>)
  {
      
  }
};
struct Bar{};
template<typename T, Impl impl>
struct Foo
{
    
};
int main()
{
    constexpr Foo<Bar, {1,2,3,4}> foo; //works in msvc & gcc but rejected in clang
    return 0;
}
