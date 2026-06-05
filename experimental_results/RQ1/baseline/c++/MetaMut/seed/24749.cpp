template<class T>
void f() {
  [](auto parm) {
    struct type {
      decltype(parm) x;
    };
  };
}

template void f<int>();

