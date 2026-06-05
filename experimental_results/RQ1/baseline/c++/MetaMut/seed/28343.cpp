int main() {
  true ? [](auto) noexcept {} : [](int) {};
}

