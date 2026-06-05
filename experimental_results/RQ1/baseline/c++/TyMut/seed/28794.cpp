
struct Foo
{
    template<class T> T get() const { return 0;  }
};

struct f
{
  int t;
  Foo t1;
};

template<typename T>
void error_func(T elem)
{
    f arr[1];
    for(const auto& [_, val] : arr)
    {
        val.get<int>(); // Error here
    }
}
