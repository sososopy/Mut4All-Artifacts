
namespace std {
  struct destroying_delete_t {
    struct __construct { explicit __construct() = default; };
  };
}

namespace delete_selection {
  struct B {
    void operator delete(void*) = delete;
    void operator delete(B *, std::destroying_delete_t) = delete;
  };
  void delete_B(B *b) { delete b; }
}

