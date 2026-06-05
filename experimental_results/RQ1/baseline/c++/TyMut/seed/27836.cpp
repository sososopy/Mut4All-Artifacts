
enum class Destination  {};
enum class Source  {};

Destination foo(Source value) { return Destination{}; }
Destination foo(double value) { return Destination{}; }

template <typename T>
struct Bar {
    Destination baz(Source aLowValue) {
        return Destination{foo(aLowValue)}; // <<<< HERE
    }
};

