

  void foo () { }
  void bar () { }

  int x[(int)(&foo != &bar)];

