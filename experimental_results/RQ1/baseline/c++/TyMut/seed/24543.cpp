struct S {
  int i = (throw 0, 0);
  S() = default;
};
int main() {
  try {
    S();
  } catch(int i) {
    return i;
  }
}

