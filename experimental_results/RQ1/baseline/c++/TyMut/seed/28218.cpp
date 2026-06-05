
#define DEF_C 1

enum foo { A, B
#if DEF_C
, C
#endif
 };
void g(const char*);

void f(foo x) {
  const char *str;
  switch (x) {
    case A: str = "a"; break;
    case B: str = "b"; break;
#if DEF_C
    case C: str = "c"; break;
#endif
  }
  g(str);
}


