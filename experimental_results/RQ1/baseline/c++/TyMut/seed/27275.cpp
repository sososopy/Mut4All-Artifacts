
void
j3 ();

template <int>
void
xm ()
{
  #pragma omp parallel master
  j3 ();
}

void
s3 ()
{
   xm<0> ();
}

