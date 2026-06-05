
#include <utility>
struct f
{
  f(const f&);
  f();
};

struct g{};

g &search(f);

void h()
{
        f n;
        const g& pt (search (std::move(n)));
}
