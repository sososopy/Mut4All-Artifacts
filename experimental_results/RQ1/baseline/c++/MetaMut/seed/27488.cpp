
struct  Algorithm
{
  Algorithm( float );
};


template <class BASE>
struct ToolWithConstants
  : public Algorithm
{
  using Algorithm::Algorithm;
  
  struct Constant
  {
    Constant (int name);
  };

  Constant c { "order" };
};

class unique_ptr {};

unique_ptr make()
{
  return unique_ptr (new ToolWithConstants<Algorithm> (10.5));
}

