
namespace test16c {
  extern "C" {
    void bar2() {} // expected-note {{previous definition is here}}
    class Foo2 {
      friend void bar2() {} // expected-error {{redefinition of 'bar'}}
    };
  }
}
