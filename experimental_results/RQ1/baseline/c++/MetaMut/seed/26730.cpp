
namespace foo {
template <class, class> constexpr bool same = false;
template <> constexpr bool same<void,void> = true;
template <class T> constexpr bool same<T,T> = true;

static_assert(same<int*,int*>, "");
static_assert(!same<int,int*>, "");
}

int main() {}

