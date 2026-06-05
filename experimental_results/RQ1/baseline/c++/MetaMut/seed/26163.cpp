template <int dim> void f(int src);

template <int dim> inline
void f(int src)    { (void)0; }

template <> inline /* AAA */
void f<2>(int src) { (void)0; }

template void f<1>(int);
template void f<2>(int);



int main(int, char** )
{
  f<1>(456);
  f<2>(789);
}
