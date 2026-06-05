
struct mine
{
  mine& operator=(mine rhs) { return *this; }
  mine& operator=(mine&& rhs) noexcept { return *this; }
};

int main()
{
  mine a;
  mine b;
  a = b;
}
