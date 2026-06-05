
template <class... Fs> 
struct overload_set : Fs...
{
  overload_set(Fs... f)
    : Fs(f)...
  {}  
};

template <class... Fs> 
overload_set<Fs...> overload(Fs... x)
{
  return overload_set<Fs...>(x...);
}

int main(void)
{
  double d = 10; 
  overload(
      [](int i)  { },
      [](double d)  { },
      [](char c) { } 
    )(d);
}
