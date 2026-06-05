
struct Nested {
    int payload;
};

struct Main {
    const Nested* nested;
    int payload;
    Main(const Nested* n) : nested(n) {}
};

void f(const Main& s)
{
}

int main() {
    f(Main(&(const Nested&)Nested()));  // <- workaround
    f(Main(&Nested()));  // <- fails here
    return 0; 
}

