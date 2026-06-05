

  namespace P{ namespace X { static int i = 1; } }
  namespace Q { namespace X = P::X; }
  using namespace P;
  using namespace Q;
  int main() { X::i; }

