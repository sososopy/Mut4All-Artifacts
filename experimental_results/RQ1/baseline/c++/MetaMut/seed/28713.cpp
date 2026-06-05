struct L{};
struct M:L{};
L l;
M m;
struct A{
    virtual L&& fun(){
        return std::move(l);
    }
};
struct B:A{
    M&  fun(){
        return m;
    }
};

