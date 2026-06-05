
using size_t = decltype(sizeof 0);
namespace std {
  template<typename T> union initializer_list {
    const T *ptr;
    size_t n;
  };
}
template<typename T>
void Task() {}
auto b = { &Task<int> };

