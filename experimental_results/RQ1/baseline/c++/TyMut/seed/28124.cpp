void
g ()
{
  float f = 1.f;
  int &r = f;
}

void
g2 ()
{
  int &r = 1.f;
}
