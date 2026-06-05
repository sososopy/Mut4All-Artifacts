
namespace std {
  template<typename T> struct initializer_list {
    const T *p; unsigned long n;
    initializer_list(const T *p, unsigned long n);
  };
}
struct vector { vector(std::initializer_list<int>); };
void f(vector);
const auto &r = {1, 2, 3};
int main() { f(r); }

