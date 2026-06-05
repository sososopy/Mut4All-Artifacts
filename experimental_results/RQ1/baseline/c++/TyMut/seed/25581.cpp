
#include <typeinfo>
#include <cstdio>

struct S {
  const char *W() {
    return typeid(this).name();
  }
  const char *X() __restrict {
    return typeid(this).name();
  }
  const char *Y() const {
    return typeid(this).name();
  }
  const char *Z() volatile {
    return typeid(this).name();
  }
};

int main() {
  S obj;
  printf("%s\n", obj.W());
  printf("%s\n", obj.X());
  printf("%s\n", obj.Y());
  printf("%s\n", obj.Z());
}

