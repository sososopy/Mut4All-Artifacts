
struct A
{
  int i;
  template<typename> void foo() : i(__PRETTY_FUNCTION__) {}
};
