template <char first, char... data>
struct CTArray
{
  static constexpr unsigned long count = sizeof... (data);
};
struct Str
{
  template <typename T>
  Str(T...) {}
};
template <typename T>
struct ToStr;
template <char... letters>
struct ToStr <CTArray <letters...>>
{
  static Str
  get ()
  {
    return { letters... };
  }
};
using Foo = CTArray <'F', 'o', 'o'>;
Str str(ToStr <Foo>::get ());
