
void
ow ();

template <int>
void
ho ()
{
#pragma omp parallel master taskloop
  ow ();
}

void
th ()
{
  ho<0> ();
}

