
template<typename F>
void runLambda(F&& f) {
  f.template operator()<3>();
  f.template operator()<7>();
}

int main(int argc, char* argv[]) {

  int retv = 0;
  constexpr bool q = false;

  runLambda([&]<int j>(){ // capturing with [&, q] always works
    constexpr bool z = j != 8;
    /*if constexpr(q) // <- this equivalent formulation works! ¯\_(ツ)_/¯
      if constexpr(z)
        retv += i*j;*/
    if constexpr(q && z)
      retv += j;
  });

  return retv;
}
