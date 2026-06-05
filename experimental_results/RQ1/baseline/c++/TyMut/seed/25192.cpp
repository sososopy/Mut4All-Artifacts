
void f (double &x)
{
  union { int x; };
  x = 0;
}
