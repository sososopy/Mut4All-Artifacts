
struct X
{
  X();
};

struct Y
{
  operator X() const;
};

X a = { Y() };    // reports error: no matching constructor for initialization of 'X'
X aa = Y();  // works fine


