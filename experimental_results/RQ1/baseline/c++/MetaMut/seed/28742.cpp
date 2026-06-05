
//$ cat foo-types.ixx
module;

export module foo:types;
export class CDerived;

//$ cat foo-test.ixx
module;
#include <iostream>
#include <typeinfo>

export module foo:test;
import :types;

export class Bar {
public:
    virtual ~Bar() = default;
    virtual void visit() {
        std::cout << "Bar::visit -> "<< typeid(*this).name() << std::endl;
    }
};

//$ cat foo.ixx
module;

export module foo;

export import :types;
export import :test;

