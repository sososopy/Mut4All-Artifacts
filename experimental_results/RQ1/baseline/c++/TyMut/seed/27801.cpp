
int a, b;
void *f;
void g (int c)
{
  b = ((int (*)[c][a]) f)[0][0][0];
}
void h (int d)
{
  g (d);
}

