
#include <iostream>
 
struct CBI;
struct EC;
struct CET;
 
struct CBI {
    CBI& operator,(const CBI& rhs)
        { return *const_cast<CBI*>(&rhs); }
};
 
struct EC : CBI {
    explicit EC(CET* cet) : cet_(cet)
        {}
 
    CET* cet_;
};
 
struct CET {
    CBI& operator,(const CBI& rhs) const
        { return *const_cast<CBI*>(&rhs); }
 
    operator EC&() const
        { return *new EC(const_cast<CET*>(this)); }
};
 
static const CET&
hello() {
    std::cout << "Hello " << std::endl;
    return *new CET();
}
 
static const CET&
world() {
    std::cout << "World " << std::endl;
    return *new CET();
}
 
static void
test_comma_operator(CBI&) {
 
}
 
int main()
{
    test_comma_operator ((
        hello(),
        world()
    ));
}
