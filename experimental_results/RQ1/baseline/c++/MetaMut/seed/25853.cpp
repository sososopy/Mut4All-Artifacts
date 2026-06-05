
template<typename T>
T declval();

template<typename T>
struct function
{
  template<typename F, typename = decltype(declval<F&>() ())>
    function(F)
    { }
};

struct V {
  typedef int value_type;
};

template <typename C>
void map(C&, function<typename C::value_type>)
{
}

int main()
{
  V v;
  map(v, []() { });
}

