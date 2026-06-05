

extern void bar1();
extern void bar2();
extern void bar3();

void foo(int i) {
  switch(i) {
  case 1:
    bar1(); // gcc: least likely | icc: most likely
    break;
  case 2:
    bar2(); // gcc: less likely | icc: less likely
    break;
  default:
    bar3(); // gcc: most likely | icc: least likely
  }
}

