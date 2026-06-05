
#include <iostream>
#include <cstdint>
#include <array>
using namespace std;


template<typename TO, typename FROM>
inline void reinterpret_switch3(TO& to, const FROM& from)
{
  to = *reinterpret_cast<const TO*>(&from);
}

template<typename TO, typename FROM>
inline void reinterpret_switch2(TO& to, const FROM& from)
{
  reinterpret_switch3(to, array<FROM, 1>({ { from } }));
}

template<typename TO, typename FROM>
inline void reinterpret_switch(TO& to, const FROM& from)
{
  array<TO, 1> tmp;
  reinterpret_switch2(tmp, from);
  to = tmp[0];
}


int main(int argc, char** argv)
{
  pair<uint32_t, uint32_t> from = {17, 23};
  pair<int32_t, int32_t> to;
  reinterpret_switch(to, from);

  cout << "from=" << from.first << "," << from.second << endl;
  cout << "to=" << to.first << "," << to.second << endl;
}
