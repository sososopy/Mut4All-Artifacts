
struct Base {
    virtual ~Base() = default;
};
struct Derived final : Base {};

bool is_derived(Base& a) {
    return dynamic_cast<Derived*>(&a);
}


