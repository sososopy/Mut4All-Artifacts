
namespace x {
  struct s {
    s() {}
    static int a;
  };
  struct s __attribute__((weak)) b = {};
};

