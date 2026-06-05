

#include <unordered_map>

struct X
{
  struct Key
  {
    int x = 0;

    bool operator==(const Key&) const = default;

    std::size_t operator()(const Key& k) const noexcept
    {
      return k.x;
    }
  };

  int a[sizeof(Key)];

  std::unordered_map<Key, std::size_t, Key> m;
};

X x;

