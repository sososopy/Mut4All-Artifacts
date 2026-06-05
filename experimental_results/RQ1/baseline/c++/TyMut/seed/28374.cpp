
export module sqrt;
export constexpr unsigned sqrt (unsigned X, unsigned x = 1)
{
  unsigned nx = (x + X/x) / 2;
  if (nx != x)
    nx = sqrt (X, nx);
  return nx;
}

