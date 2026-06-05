

struct base {
    base (int const &);
};

struct derived : public base {
    using base::base;
};

derived d(0);

