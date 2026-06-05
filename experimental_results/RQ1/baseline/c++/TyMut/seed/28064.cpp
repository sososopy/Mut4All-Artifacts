

// g++ -std=c++1z
struct alignas(8) data {
  constexpr data(bool) : value(true) {}
  
  // shouldn't be called. change to 'default' to see miscompile.
  data() = delete;
  
  bool value;
  // ... implicit padding ...
};

struct wrap { data member; };

constexpr bool always_true() {
    wrap w{data(true)};
    w.member = data(true);
    return w.member.value; // should always return true
}

bool test() {
    // emits-error {{use of deleted function 'data::data()'}}
    // emits-note {{in 'constexpr' expansion of 'always_true()' }}
    return always_true();

    // returns 0 when miscompiled.
}
