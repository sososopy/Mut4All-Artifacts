
void
foo ()
{
  float t[2] = { 1, 2 };
  int const *s = 0;
  t[1] / s;
}
void
bar ()
{
  float t[2] = { 1, 2 };
  int const *s[2] = { 0, 0 };
  t[1] / s[0];
}

