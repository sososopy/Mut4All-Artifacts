
template <class>
void
qt_fetch_radial_gradient_template (int)
{
  extern void (*qt_memfill32)(int, int, int);
  qt_memfill32 (0, 0, 0);
}

void
qt_fetch_radial_gradient_sse2 ()
{
  qt_fetch_radial_gradient_template<int> (0);
}

