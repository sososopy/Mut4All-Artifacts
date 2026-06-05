

struct NonCopyable {
  NonCopyable() {}
  NonCopyable(const NonCopyable&) = delete;
};

template <class... Args>
void Construct(Args... args) {
  NonCopyable obj(args...);
  (void)obj;
}

int main() {
  Construct();
}

