
template<class> void f()
{
  auto a = [] (auto x) static { return x; };
}
template void f<int>();

