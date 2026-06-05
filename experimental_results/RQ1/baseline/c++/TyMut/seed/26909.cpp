
struct RE
{
  virtual void rp()=0;
  void ax(){rp();}
};

struct BLKC
{
  virtual void rb(){}
};

template <typename T>
struct LK : BLKC
{
  T* p = nullptr;
  void rb() override { p->ax();}
};

template <typename>
struct AN : RE
{
  void rp() override {}
};

template <const char*>
struct LR
{
  virtual ~LR(){}
  struct LLC { virtual ~LLC(){} };
  LK<AN<LLC>> l;
};

constexpr char ET[]="";
struct I
{
  LR<ET> _e;
};

int main(){new I();}


