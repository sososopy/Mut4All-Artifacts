struct S {
    enum E { A, B } e : 1;                   // line 2
    void f1(decltype(e)) {}                  // line 3
    template <typename> void f2() { f1(e); } // line 4
};

