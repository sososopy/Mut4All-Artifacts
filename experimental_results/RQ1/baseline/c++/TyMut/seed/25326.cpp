struct move_only {
    move_only() = default;
    move_only(move_only&&) = default;
};

struct base {
    base(move_only) {}
};

struct derived: base {
    // error: use of deleted function 'constexpr move_only::move_only(const move_only&)'
    using base::base;
};

int main()
{
    // note: synthesized method 'derived::derived(move_only)' first required here
    derived h(move_only {});
}

