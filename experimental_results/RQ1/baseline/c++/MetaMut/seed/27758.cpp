
namespace std {
  using size_t = decltype(sizeof(0));
  enum class align_val_t : size_t;

  struct destroying_delete_t { };
}

struct A {
  void operator delete(A*, std::destroying_delete_t, std::align_val_t);
  void operator delete(A*, std::destroying_delete_t, std::size_t, std::align_val_t);
};

void delete_A(A *a) { delete a; }

