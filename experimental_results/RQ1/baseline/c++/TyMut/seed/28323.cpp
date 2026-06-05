
extern "C" double sqrt(double);
double a, b, c;
void d() {
  for (;;) {
    c = __builtin_fabs(a);
    sqrt(c);
    if (a)
      a = b;
  }
}

