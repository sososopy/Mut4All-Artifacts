
template<typename T, typename T1> 
int f (T x, T1 y) throw ()
{
    return 3;
}

int
main ()
{
  return (f (0.0f64x, 0.0f64x) + f (0.0f64x, 0.0L)) != 6;
}
