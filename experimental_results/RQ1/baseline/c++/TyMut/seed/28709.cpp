template<class... Ts>
struct Storage
{
  using Args = int;
};

template<class... Args>
void delegate(Args&&... args)
{
  using X = typename Storage<Args...>::Args; // reject this
}

