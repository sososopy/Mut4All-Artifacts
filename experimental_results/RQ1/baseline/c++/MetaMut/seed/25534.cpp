
namespace test16b {
  extern "C++" {
    class Foo;
    extern "C" int bar(Foo *y);
    class Foo {
      int x;
      friend int bar(Foo *y);
    };
    extern "C" int bar(Foo *y) {
      return y->x;
    }
  }
}

namespace test16c {
  extern "C" {
    class Foo;
    extern "C++" int bar(Foo *y);
    class Foo {
      int x;
      friend int bar(Foo *y);
    };
    extern "C++" int bar(Foo *y) {
      return y->x;
    }
  }
}
