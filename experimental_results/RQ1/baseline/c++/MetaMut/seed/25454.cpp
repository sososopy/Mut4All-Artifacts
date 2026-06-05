
int main() {
  union {
    int a;
    int &b = a;
  };
  a = 1;
  auto c = b + 1;
}

