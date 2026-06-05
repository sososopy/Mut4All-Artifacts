
struct W {
  int a;
  ~W() { a = 9; }
};

int main() {
 W w {};
 W v = w;
}
