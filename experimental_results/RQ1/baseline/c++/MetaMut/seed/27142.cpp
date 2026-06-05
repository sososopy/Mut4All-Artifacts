

template <typename... Args>
int sink(Args&&... args) { return 2; }

template <typename T1>
auto fwd(const T1& t1) {
  return
    [] (auto&&... ts1) {
      return
        [...ts1 = ts1] () {
          return sink(ts1...);
        }();
    }();
}

int main() {
  return fwd(1);
}
