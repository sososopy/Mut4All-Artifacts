
  export module M;
  export namespace ns {
    template <typename T> void foo() {}
    template <> void foo<int>() {}
  }


  extern "C++" namespace ns {
    struct Incomplete;
    Incomplete bar;
  }


