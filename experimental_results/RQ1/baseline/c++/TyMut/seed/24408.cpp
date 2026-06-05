
template <class Ty1>
class base
{
public:
    std::atomic_size_t _magic{10};
};

class derived
    : public base<derived>
{
public:
    auto print_magic_num() const noexcept -> void
    {
    }
};

auto my_func() -> void
{
    derived a;
}

