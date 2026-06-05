
void foo(int i)
{
  typedef int I;
  __decltype(i.I::~I())* p;
}
