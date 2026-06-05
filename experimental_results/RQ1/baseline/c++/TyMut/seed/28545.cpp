
template<typename T>
class G
{
public:
    auto static F() { return 1; }
};

int main()
{
    auto fn = [](auto const& f) -> void { f(); };

    fn(G<double>::F);

    return 0;
}
