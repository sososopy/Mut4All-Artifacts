
template<typename> void foo()
{
  int i __attribute__((vector_size(8)));
  __builtin_shuffle(i, i);
}
