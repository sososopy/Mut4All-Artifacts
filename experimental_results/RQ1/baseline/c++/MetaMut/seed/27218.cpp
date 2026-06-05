
struct [[nodiscard]] X {
    int x;
    X operator+() const { return *this; }
};
class [[nodiscard]] Y {
    int y;
public:
    Y operator+() const { return *this; }
};
void f()
{
    X{};
    Y{};
    X x;
    +x;
    Y y;
    +y;
}
