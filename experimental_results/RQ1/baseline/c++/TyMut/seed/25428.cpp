
template <typename, typename _Alloc> class vector {
public:
  vector();
  vector(const _Alloc &) {}
  void operator=(vector) { vector a(get_allocator()); }
  _Alloc get_allocator();
};

struct inherent;
template <typename, typename = inherent, typename = decltype(nullptr)>
struct allocator_affinity {};

typedef vector<void *, allocator_affinity<void *>> addr_list_t;

struct A {
  A(int) {}
  addr_list_t req_start_h_0;
  void req_start() { req_start_h_0 = addr_list_t(); }
};

int b;
void fn1() {
  A a(b);
  a.req_start();
}

