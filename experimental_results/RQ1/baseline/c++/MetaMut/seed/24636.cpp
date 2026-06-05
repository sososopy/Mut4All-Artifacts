
void foo()
{}
  int i;
#pragma GCC ivdep
  for (i = 0; i < 2; ++i)
    ;
}
