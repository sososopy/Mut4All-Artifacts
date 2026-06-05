
int f(int, int);
int f(int);

template<class...Args>
auto select(Args... args) -> decltype(f(args...))
{
  if constexpr(sizeof...(Args)>1) 
    return select<char>(7);
  else
    return 0;
}

int a = select(0, 1);


