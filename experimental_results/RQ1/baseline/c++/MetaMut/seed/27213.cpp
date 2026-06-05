
auto yp = [] { return 0; };

template <class DI>
DI
zl ()
{
  auto au = [] () -> DI { return *new auto (true ? yp : throw); };

  return au ();
}

auto
vd ()
{
  return zl <decltype (yp)> ();
}

