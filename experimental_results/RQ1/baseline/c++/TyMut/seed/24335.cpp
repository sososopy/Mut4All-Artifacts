
template <class T, class U> struct mypair {
  mypair(T, U) {}
};

template <class T, class U> auto my_make_pair(T t, U u)
{
  return mypair<T, U>(t, u);
}

template<typename T> struct S {
 mypair<T *, int> get_pair() noexcept { 
   return my_make_pair((T*)nullptr, 0); 
 } 
}; 

static void foo(const mypair<char *, int> (&a)[2]) noexcept { } 

int main()
{ 
  S<char> s; 
  //mypair<char*, int> jones[2]{s.get_pair(), s.get_pair()};
  //foo(jones);
  foo({s.get_pair(), s.get_pair()}); 
}

