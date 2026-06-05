
struct bar
{
  bar() = default;
  bar(const bar&) = delete;
};

int main()
{
  bar b;
  const bar& b1_(b);
  const bar& b2_{b};
}
