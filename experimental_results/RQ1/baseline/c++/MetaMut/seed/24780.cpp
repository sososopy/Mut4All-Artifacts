
struct tag { };

struct foo {
    using tag = tag;
};

struct bar { 
    using tag = tag;
};

struct foobar : foo, bar { };

int main() {
    foobar::tag _;
}
