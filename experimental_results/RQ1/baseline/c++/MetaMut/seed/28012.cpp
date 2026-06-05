
#include <memory>

class Base
{
public:
    Base(int = 0);
    virtual ~Base();
};

class DerivedPrivate;
class Derived final : public Base
{
public:
    using Base::Base;

    ~Derived() override;

private:
    std::unique_ptr<DerivedPrivate> d;
};

