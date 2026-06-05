

struct SomeClass {
  int someFunction() { return 1; }
};

struct SomeFunctor {
  using MemberFunction = int (SomeClass::*)();

  constexpr explicit SomeFunctor(const MemberFunction memberFunction)
      : memberFunction_{memberFunction} {}

  MemberFunction memberFunction_;
};

SomeFunctor functor{&SomeClass::someFunction};

int main() {
  if (!functor.memberFunction_)
    __builtin_abort();
}


