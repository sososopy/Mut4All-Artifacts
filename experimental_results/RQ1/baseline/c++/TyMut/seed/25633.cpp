
template<int> void foo()
{
  enum E {};
  E().E::~T();
}
