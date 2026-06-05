
enum class fruit { orange, apple };

struct Working {
    using enum fruit;
};

struct Broken {
    using enum fruit;
private:
};

int main() {
    Working::orange;
    Broken::orange;
}

