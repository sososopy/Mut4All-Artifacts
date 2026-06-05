
#include <utility>
#include <unordered_map>

using namespace std;

template <typename T, typename U>
struct hash<pair<T, U> > {
 public:
  size_t operator()(pair<T, U> x) const throw() {
    return hash<T>()(x.first) ^ hash<U>()(x.second);
  }
};

int main(int argc, char *argv[]) {
  return 0;
}
