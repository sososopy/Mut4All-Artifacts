
void foo()
{
  int v __attribute__((vector_size(8)));
  v = v || v;
}
