
class B
{
public:
  virtual ~B() noexcept(false) { }
};

class D : public B
{
public:
  virtual ~D() = default;
};

