
#include <memory>

class AbstractClass
{
    virtual ~AbstractClass() {}
    virtual void f() = 0;
};

class MakeSharedInternalCompilerError
{
    std::shared_ptr<AbstractClass> m_member = std::make_shared<AbstractClass>();
};
