

int main() {
  static constexpr const void *vp = nullptr;
  static constexpr const char *cp = static_cast<const char*>(vp);
}
