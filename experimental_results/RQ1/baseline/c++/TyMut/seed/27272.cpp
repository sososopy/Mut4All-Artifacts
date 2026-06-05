
int func(int *x) {
  for (int i = 0; i < 23; ++i)
    co_yield x[i];
}

