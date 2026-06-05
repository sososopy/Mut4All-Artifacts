
struct C
{
    constexpr int fun (int x)
    {
        return x + 1;
    }

    int a = 2;
    int b = fun(a);
};

C c;

int main() {}

