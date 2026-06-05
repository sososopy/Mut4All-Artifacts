
class X {
};

template <typename T>
class MyCl {
  T mem;
};

class Source {
  MyCl<X> m;
public:
  int getKind() const;
};

bool b;
template<typename TT>
static void foo(const Source &SF, MyCl<TT *> Source::*m) {
  switch (SF.getKind()) {
    case 1: return;
    case 2: break;
    case 3:
    case 4: return;
  };
  if (b) {
    auto &y = const_cast<MyCl<TT *> &>(SF.*m);
  }
}

int main(int argc, char **argv) {
}

