
struct Base {
    virtual void v();
};
struct Derived: Base {
    virtual void v(); // warning happens here
};

