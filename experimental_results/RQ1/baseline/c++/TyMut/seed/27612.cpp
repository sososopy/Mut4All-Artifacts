
template <typename _UnaryOperation>
void transform(_UnaryOperation);

template <typename T>
void Apply ()
{
  extern T Maker (void);  // block-scope extern with dependent type

  transform (Maker);
}

template void Apply<int> ();

