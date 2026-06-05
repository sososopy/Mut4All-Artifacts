

struct ptr
{
  int* p;

  explicit operator bool() const { return p != nullptr; }
};

int main()
{
  ptr u{};
  new int(u);
}
