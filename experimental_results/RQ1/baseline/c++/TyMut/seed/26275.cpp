
template<typename T> void foo(T* p)
{
  p->decltype(T{})::~X();
};
