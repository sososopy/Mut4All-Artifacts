
template<typename T>
struct A
{
  typedef T type;
};

template<typename T>
struct B
{};

template<typename T>
B<typename A<T const>::type>
f(T const &)
{}

template<typename CharT>
struct X
{
  typedef CharT char_type;

  void memfun()
  {
    char_type const c[1] = { static_cast<char_type>(0) };
    f(c);
  }
};

template<typename CharT>
struct Y
{
  typedef CharT char_type;

  void memfun()
  {
    char_type const c[1] = { static_cast<char_type>(0) };
    f(c);
  }
};

template struct X<char>;
template struct Y<char>;
