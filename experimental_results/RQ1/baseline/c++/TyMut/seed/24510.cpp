
struct function
{
    template < typename _Functor > function (_Functor);
};

template < typename > struct RetryingRpc
{
  template < typename StubType> RetryingRpc (StubType, function =[]{});
};

void fn()
{
  RetryingRpc<int> rpc(0, []{});
};
