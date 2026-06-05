
struct span {
  template<class T> constexpr span(T) { }
};

struct byte_writer : span { };

void f(char *p) {
  byte_writer w{p};
}

int main() { }


