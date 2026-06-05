
struct [[nodiscard]] S{};

void f();

void _()
{
    f(), S{};
}
