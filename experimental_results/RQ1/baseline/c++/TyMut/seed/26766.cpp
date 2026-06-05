

template <class>
void tf()
{
  extern void (*qt_memfill32)();
  qt_memfill32();
}

void f() {
  tf<int>();
}

