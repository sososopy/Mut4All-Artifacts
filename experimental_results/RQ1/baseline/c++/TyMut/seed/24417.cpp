
constexpr const int intval = 0;
constexpr const char chararray[] = "test";

template<const char*>
struct charptrbase {};

template<const int*>
struct intptrbase {};

template<int>
struct intbase {};


struct child1 : public charptrbase<chararray>
{};

struct child2 : public intptrbase<&intval>
{};

struct child3 : public intbase<intval>
{};
# 2 "test.cpp" 2

int main()
{
  child2 foo;
  (void)foo;
  return 0;
}
