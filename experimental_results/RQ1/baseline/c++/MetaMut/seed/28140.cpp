
  #include <valarray>

  struct TTensor {};

  struct TCoefs
  {
    double aP;
    std::valarray<double> aF;
    TTensor b;
  };

  TCoefs CalcCoefs()
  {
  double aP = 0.0;
  return {aP, {}, {}};
  }

