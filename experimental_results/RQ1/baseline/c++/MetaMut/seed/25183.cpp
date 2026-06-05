
struct foo {
  int a;
  char s[];
};

struct bar {
  double d;
  char t[];
};

struct baz {
  union {
    struct foo f;
    struct bar b;
  } u;
};

struct xyyzy {
  union {
    struct {
      int a;
      char s[];
    } f;
    struct {
      double d;
      char t[];
    } b;
  } u;
};

struct baz b;
struct xyyzy x;
