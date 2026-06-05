
class base
{
protected:
  ~base () {}
  virtual void foo (){};
};
class derive final : public base
{
};

