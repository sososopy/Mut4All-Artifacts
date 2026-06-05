
static_assert(std::is_trivially_copyable_v<std::optional<char>>);
static void not_ok() {
  std::optional<char> value;
  std::byte buf[128;
  std::memcpy(&buf[0], &value, sizeof value);
  std::memcpy(&value, &buf[0], sizeof value);
}

