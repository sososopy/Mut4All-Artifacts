struct S {
  auto f(this auto self...) {  }
};

int main() {
  S{}.f();
}

