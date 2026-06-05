

struct base { ~base() {} };
struct function : base { function(...) {} };
struct thing {};
struct another {
  another() : r(thing()) {}
  const function &r;
} a;
