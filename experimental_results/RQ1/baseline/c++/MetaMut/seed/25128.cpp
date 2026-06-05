
struct A;
using variant_nil = int;
template <typename Fun, typename Proj = A>
void variant_visit_(int, variant_nil, Fun, Proj = {});
struct A {};
void get() {
  variant_visit_(0, 0, int{});
}
