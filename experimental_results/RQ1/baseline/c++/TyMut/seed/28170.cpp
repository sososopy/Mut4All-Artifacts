

template<typename T> T&& declval();

template<typename Arg> struct function
{
  template<typename F, typename = decltype(declval<F>()(declval<Arg>()))>
    function(F) { }
};

template<typename T>
struct test
{
  function<T*> f = [](T *) {};
};

struct test2
{
  test<double> d;
  test<int> i;
};

template<typename T> void make() { new T(); }

void g ()
{
  make<test2>();
}
