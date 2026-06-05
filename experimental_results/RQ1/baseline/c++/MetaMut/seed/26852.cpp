
  namespace X { extern int i; }

  namespace N { using X::i; }

  int N::i = 1;

  int main() {}

